
#include "oaLuaBehavior.h"
#include "oaTransform.h"
#include "oaEntity.h"

oaLuaBehavior::oaLuaBehavior(oaComponent component) : oaLuaBehavior() {
	_entityID = component.GetEntityID();
	_id = component.GetID();
}

oaLuaBehavior::oaLuaBehavior() {
	luaState = NULL;
}

void oaLuaBehavior::awake() {
	initializeState();
}

void oaLuaBehavior::start() {
}

void oaLuaBehavior::update() {
	//if (!luaState.) initializeState();

	// registerMethods();
	luaState.script_file(luaScriptPath);
	//if (luaL_dofile(luaState, luaScriptPath.c_str())) {
	//	printf("%s (error):: %s \n", luaScriptPath.c_str(), lua_tostring(luaState, -1));
	//	return;
	//}
}

void oaLuaBehavior::onDestroy() {
	//if (luaState)
	//	lua_close(luaState);
}

void oaLuaBehavior::initializeState() {
	//luaState = luaL_newstate();
	//luaL_openlibs(luaState);
	//luaopen_base(luaState);
	//luaopen_string(luaState);
	registerMethods();

	sol::load_result script1 = luaState.load_file(luaScriptPath);
	//if (luaL_loadfile(luaState, luaScriptPath.c_str()) || lua_pcall(luaState, 0, 0, 0)) {
	//	printf("%s (error):: %s \n", luaScriptPath.c_str(), lua_tostring(luaState, -1));
	//	luaState = NULL;
	//	return;
	//}
}

void oaLuaBehavior::registerMethods() {
	luaState.open_libraries(sol::lib::base);
	luaState.set_function("_getEntityID", &oaLuaBehavior::GetEntityID, this);
	luaState.set_function("_getID", &oaLuaBehavior::GetID, this);
	// lua_register(luaState, "moveEntity", _lua_moveEntity);
}

REGISTER_CLASS(oaLuaBehavior)
