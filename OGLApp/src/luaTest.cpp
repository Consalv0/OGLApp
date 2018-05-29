#include "luaTest.h"
#include "oaEntity.h"
#include "oaTransform.h"
#include "oaMeshRenderer.h"

luaTest::luaTest(oaComponent component) : oaLuaBehavior(component) {
}

luaTest::luaTest() : oaLuaBehavior() {
}

void luaTest::awake() {
	initializeState();
}

void luaTest::start() {
}

void luaTest::update() {
	oaLuaBehavior::update();
}

void luaTest::onDestroy() {
}

void luaTest::initializeState() {
	oaLuaBehavior::initializeState();
}

void luaTest::registerMethods() {
	oaLuaBehavior::registerMethods();

	luaState.set_function("_movePosition", &luaTest::movePosition, this);
	luaState.set_function("_setPosition", &luaTest::setPosition, this);
	luaState.set_function("_getPosition", &luaTest::getPosition, this);
	luaState.set_function("_rotate", &luaTest::rotate, this);
	luaState.set_function("_setRoughness", &luaTest::setRoughness, this);
	luaState.set_function("_setMetalness", &luaTest::setMetalness, this);
	luaState.set_function("_setLightIntensity", &luaTest::setLightIntensity, this);
	luaState.set_function("_getLightIntensity", &luaTest::getLightIntensity, this);
	luaState.set_function("_castShadows", &luaTest::castShadows, this);
	luaState.set_function("_lookAt", &luaTest::lookAt, this);
	luaState.set_function("_setAmbientLight", &luaTest::setAmbientLight, this);
}

void luaTest::movePosition(float x, float y, float z) {
	transform()->position += glm::vec3(x, y ,z);
}

void luaTest::setPosition(float x, float y, float z) {
	transform()->position = glm::vec3(x, y, z);
}

void luaTest::rotate(float x, float y, float z) {
	transform()->rotation *= glm::quat(glm::vec3(x, y, z));
}

std::tuple<float, float, float> luaTest::getPosition() {
	glm::vec3 pos = transform()->position;
	return std::make_tuple(pos.x, pos.y, pos.z);
}

void luaTest::setRoughness(float i) {
	auto behavior = getComponent<oaMeshRenderer>();
	if (behavior)
		behavior->material->roughness = i;
}

void luaTest::setMetalness(float i) {
	auto behavior = getComponent<oaMeshRenderer>();
	if (behavior)
		behavior->material->metallic = i;
}

void luaTest::setAmbientLight(float x, float y, float z) {
	auto behavior = getComponent<oaMeshRenderer>();
	if (behavior)
		behavior->material->ambientColor = {x, y, z};
}

void luaTest::lookAt(float x, float y, float z) {
	transform()->lookAt({x, y, z});
}

float luaTest::getLightIntensity() {
	auto light = getComponent<oaMeshRenderer>()->light;
	if (light) {
		return light->intensity;
	}

	return NULL;
}

void luaTest::setLightIntensity(float i) {
	auto light = getComponent<oaMeshRenderer>()->light;
	if (light) {
		light->intensity = i;
	}
}

void luaTest::castShadows(bool b) {
	auto light = getComponent<oaMeshRenderer>()->light;
	if (light) {
		light->castShadows = b;
	}
}

REGISTER_CLASS(luaTest)