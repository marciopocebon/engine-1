/**
 * @file
 */

#include "Noise.h"
#include "core/Trace.h"
#include "core/Log.h"
#include "core/Common.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <limits>

#define GLM_NOISE 0
#define CINDER_NOISE 1

namespace noise {

/**
 * @brief Fractional Brownian Motion
 *
 * fBM (fractional Brownian motion) is a composite Perlin noise algorithm. It creates more turbolence with more octaves.
 *
 * To cover all possible scales, the octaves are typically a bit less than @code log(width) / log(lacunarity)@endcode.
 * So, for a 1024x1024 heightfield, about 10 octaves are needed. The persistence influences the terrain turbolence.
 *
 * @param[in] octaves The amount of octaves controls the level of detail. Adding more octaves increases the detail level, but also the computation time.
 * @param[in] persistence A multiplier that defines how fast the amplitude diminishes for each successive octave.
 * @param[in] lacunarity A multiplier that defines how quickly the frequency changes for each successive octave.
 * @param[in] amplitude The maximum absolute value that the noise function can output.
 */
template<class VecType>
static float NoiseFBM(const VecType& pos, int octaves, float persistence, float lacunarity, float frequency, float amplitude) {
	core_trace_scoped(Noise);
	float total = 0.0f;
	for (int i = 0; i < octaves; ++i) {
#if GLM_NOISE == 1
		total += glm::simplex(pos * frequency) * amplitude;
#elif CINDER_NOISE == 1
		total += noise(pos * frequency) * amplitude;
#endif
		frequency *= lacunarity;
		amplitude *= persistence;
	}
	return total;
}

Noise::Noise() :
		_shader(compute::NoiseShader::getInstance()) {
}

Noise::~Noise() {
	shutdown();
}

bool Noise::init() {
	_useShader = _shader.setup();
	if (_useShader) {
		Log::debug("Noise shaders can be used");
	} else {
		Log::debug("Noise shaders can't be used");
	}
	return true;
}

void Noise::shutdown() {
	_useShader = false;
	_shader.shutdown();
}

bool Noise::useShader(bool enableShader) {
	if (!_useShader && _enableShader) {
		Log::warn("Can't enable use of shaders, not supported or not initialized");
		return false;
	}
	_enableShader = enableShader;
	return true;
}

float Noise::fbmNoise2D(const glm::vec2& pos, int octaves, float persistence, float frequency, float amplitude) const {
	return NoiseFBM(pos, octaves, persistence, 2.0f, frequency, amplitude);
}

float Noise::fbmNoise3D(const glm::vec3& pos, int octaves, float persistence, float frequency, float amplitude) const {
	return NoiseFBM(pos, octaves, persistence, 2.0f, frequency, amplitude);
}

float Noise::fbmNoise4D(const glm::vec4& pos, int octaves, float persistence, float frequency, float amplitude) const {
	return NoiseFBM(pos, octaves, persistence, 2.0f, frequency, amplitude);
}

int32_t Noise::intValueNoise(const glm::ivec3& pos, int32_t seed) const {
	constexpr int32_t xgen = 1619;
	constexpr int32_t ygen = 31337;
	constexpr int32_t zgen = 6971;
	constexpr int32_t fixedseed = 1013;
	int32_t n = (xgen * pos.x + ygen * pos.y + zgen * pos.z + fixedseed * seed) & std::numeric_limits<int32_t>::max();
	n = (n >> 13) ^ n;
	return (n * (n * n * 60493 + 19990303) + 1376312589) & std::numeric_limits<int32_t>::max();
}

double Noise::doubleValueNoise(const glm::ivec3& pos, int32_t seed) const {
	constexpr double div = static_cast<double>(std::numeric_limits<int32_t>::max() / 2 + 1);
	const int ni = intValueNoise(pos, seed);
	const double n = static_cast<double>(ni / div);
	return 1.0 - glm::abs(n);
}

double Noise::voronoi(const glm::dvec3& pos, bool enableDistance, double frequency, int seed) const {
	const glm::dvec3 p = pos * frequency;
	const glm::ivec3 rp(
			(p.x > 0.0 ? (int)(p.x) : (int)(p.x) - 1),
			(p.y > 0.0 ? (int)(p.y) : (int)(p.y) - 1),
			(p.z > 0.0 ? (int)(p.z) : (int)(p.z) - 1));

	double minDist = static_cast<double>(std::numeric_limits<int32_t>::max());
	glm::dvec3 vp(0.0);

	const int d = 2;
	for (int z = rp.z - d; z <= rp.z + d; ++z) {
		for (int y = rp.y - d; y <= rp.y + d; ++y) {
			for (int x = rp.x - d; x <= rp.x + d; ++x) {
				const glm::ivec3 c(x, y, z);
				const double noiseX = x + doubleValueNoise(c, seed);
				const double noiseY = y + doubleValueNoise(c, seed + 1);
				const double noiseZ = z + doubleValueNoise(c, seed + 2);
				const glm::dvec3 noisePos(noiseX, noiseY, noiseZ);
				//const double dist = glm::abs(noiseX) + glm::abs(noiseY) + glm::abs(noiseZ) + glm::length2(noisePos);
				const double dist = glm::length2(noisePos - p);
				if (dist < minDist) {
					minDist = dist;
					vp = noisePos;
				}
			}
		}
	}

	const double value = enableDistance ? glm::length(vp - p) * glm::root_three<double>() - 1.0 : 0.0;
	const double ret = value + doubleValueNoise(glm::ivec3(glm::floor(vp)));
	return ret;
}

float Noise::swissTurbulence(const glm::vec2& p, float offset, int octaves, float lacunarity, float gain, float warp) const {
	float sum = 0.0f;
	float freq = 1.0f;
	float amp = 1.0f;
	glm::vec2 dsum(0.0f);
	for (int i = 0; i < octaves; i++) {
		const glm::vec2 in(p + glm::vec2(offset + i) + warp * dsum);
		const glm::vec3& n = noise::dnoise(in * freq);
		sum += amp * (1.0f - glm::abs(n.x));
		dsum += amp * glm::vec2(n.y, n.z) * -(n.x * 1.5f);
		freq *= lacunarity;
		amp *= gain * glm::clamp(sum, 0.0f, 1.0f);
	}
	return (sum - 1.0f) * 0.5f;
}

float Noise::jordanTurbulence(const glm::vec2& p, float offset, int octaves, float lacunarity, float gain1, float gain, float warp0, float warp, float damp0, float damp, float damp_scale) const {
	glm::vec3 n = dnoise(p + glm::vec2(offset));
	glm::vec3 n2 = n * n.x;
	float sum = n2.x;
	glm::vec2 dsumWarp = warp0 * glm::vec2(n2.y, n2.z);
	glm::vec2 dsumDamp = damp0 * glm::vec2(n2.y, n2.z);

	float amp = gain1;
	float freq = lacunarity;
	float dampedAmp = amp * gain;

	for (int i = 1; i < octaves; i++) {
		const glm::vec2 in((p + glm::vec2(offset + i / 256.0f)) * freq + glm::vec2(dsumWarp));
		n = noise::dnoise(in);
		n2 = n * n.x;
		sum += dampedAmp * n2.x;
		dsumWarp += warp * glm::vec2(n2.y, n2.z);
		dsumDamp += damp * glm::vec2(n2.y, n2.z);
		freq *= lacunarity;
		amp *= gain;
		dampedAmp = amp * (1 - damp_scale / (1 + dot(dsumDamp, dsumDamp)));
	}
	return sum;
}

void Noise::seamlessNoise(uint8_t* buffer, int size, int octaves, float persistence, float frequency, float amplitude) const {
	const int components = 3;
	if (canUseShader()) {
		const glm::ivec2 workSize(size);
		_shader.seamlessNoise(buffer, size * size * components, size, components, octaves, persistence, amplitude, workSize);
		return;
	}
	uint8_t bufferChannel[size * size];
	const float pi2 = glm::two_pi<float>();
	const float d = 1.0f / size;
	for (int channel = 0; channel < components; ++channel) {
		// seamless noise: http://www.gamedev.net/blog/33/entry-2138456-seamless-noise/
		float s = 0.0f;
		for (int x = 0; x < size; x++, s += d) {
			const float s_pi2 = s * pi2;
			const float nx = glm::cos(s_pi2);
			const float nz = glm::sin(s_pi2);
			float t = 0.0f;
			for (int y = 0; y < size; y++, t += d) {
				const float t_pi2 = t * pi2;
				const float ny = glm::cos(t_pi2);
				const float nw = glm::sin(t_pi2);
				float noise = fbmNoise4D(glm::vec4(nx, ny, nz, nw) + glm::vec4(channel), octaves, persistence, frequency, amplitude);
				noise = noise::norm(noise);
				const unsigned char color = (unsigned char) (noise * 255.0f);
				const int channelIndex = y * size + x;
				bufferChannel[channelIndex + 1] = color;
			}
		}
		int index = 0;
		for (int x = 0; x < size; ++x) {
			for (int y = 0; y < size; ++y, ++index) {
				buffer[index * components + channel] = bufferChannel[index];
			}
		}
	}
}

}
