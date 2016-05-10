/**
 * @file
 */

#include "BiomeManager.h"

namespace voxel {

constexpr Voxel BiomeManager::INVALID;
constexpr Voxel BiomeManager::ROCK;
constexpr Voxel BiomeManager::GRASS;
constexpr Biome BiomeManager::DEFAULT;

BiomeManager::BiomeManager() {
}

BiomeManager::~BiomeManager() {
}

bool BiomeManager::addBiom(int lower, int upper, float humidity, float temperature, const Voxel& type) {
	bioms.emplace_back(type, int16_t(lower), int16_t(upper), humidity, temperature);
	return true;
}

const Biome* BiomeManager::getBiome(const glm::ivec3& pos, float noise) const {
	core_assert_msg(noise >= 0.0f && noise <= 1.0f, "noise must be normalized [-1.0,1.0]: %f", noise);
	const glm::vec2 noisePos(pos.x * noise, pos.z * noise);
	const float humidityNoise = noise::Simplex::Noise2D(noisePos, 1, 1.0f, 1.0f, 1.0f);
	const float temperatureNoise = noise::Simplex::Noise2D(noisePos, 1, 1.2f, 1.2f, 1.2f);
	const float humidityNoiseNorm = noise::norm(humidityNoise);
	const float temperatureNoiseNorm = noise::norm(temperatureNoise);

	const Biome *biomeBestMatch = &DEFAULT;
	float distMin = std::numeric_limits<float>::max();

	for (const Biome& biome : bioms) {
		if (pos.y > biome.yMax || pos.y < biome.yMin) {
			continue;
		}
		const float dTemperature = temperatureNoiseNorm - biome.temperature;
		const float dHumidity = humidityNoiseNorm - biome.humidity;
		const float dist = (dTemperature * dTemperature) + (dHumidity * dHumidity);
		if (dist < distMin) {
			biomeBestMatch = &biome;
			distMin = dist;
		}
	}
	return biomeBestMatch;
}

bool BiomeManager::hasTrees(const glm::ivec3& pos, float noise) const {
	if (pos.y < MAX_WATER_HEIGHT) {
		return false;
	}
	const Biome* biome = getBiome(pos, noise);
	if (!isGrass(biome->voxel.getMaterial())) {
		return false;
	}
	return biome->temperature > 0.3f && biome->humidity > 0.3f;
}

bool BiomeManager::hasClouds(const glm::ivec3& pos, float noise) const {
	if (pos.y <= MAX_MOUNTAIN_HEIGHT) {
		return false;
	}
	const Biome* biome = getBiome(pos, noise);
	return biome->humidity >= 0.6f;
}

}
