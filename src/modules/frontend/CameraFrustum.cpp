#include "CameraFrustum.h"
#include "video/Renderer.h"

namespace frontend {

bool CameraFrustum::init(const video::Camera& frustumCamera, const glm::vec4& color, int splitFrustum) {
	_splitFrustum = splitFrustum;
	_shapeRenderer.init();
	_shapeBuilder.setColor(color);
	_shapeBuilder.frustum(frustumCamera, _splitFrustum);
	_frustumMesh = _shapeRenderer.create(_shapeBuilder);
	return true;
}

void CameraFrustum::shutdown() {
	_shapeRenderer.shutdown();
	_shapeBuilder.shutdown();
}

void CameraFrustum::render(const video::Camera& camera, const video::Camera& frustumCamera) {
	_shapeBuilder.clear();
	_shapeBuilder.frustum(frustumCamera, _splitFrustum);
	_shapeRenderer.update(_frustumMesh, _shapeBuilder);
	if (_renderAABB) {
		const math::AABB<float>& aabb = frustumCamera.aabb();
		_shapeBuilder.clear();
		_shapeBuilder.aabb(aabb);
		_shapeRenderer.createOrUpdate(_aabbMesh, _shapeBuilder);
	} else if (_aabbMesh >= 0) {
		_shapeRenderer.deleteMesh(_aabbMesh);
		_aabbMesh = -1;
	}
	_shapeRenderer.renderAll(camera);
}

}
