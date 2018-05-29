#pragma once

#include <yaml-cpp\yaml.h>
#include <memory>
#include <string>
#include <map>

class oaObject;

/* Makes a method to get the derived class in a shared pointer */
#define REGISTER_INSTANCE_METHOD(BASECLASS, CLASS) \
virtual std::shared_ptr<BASECLASS> make_instance() { return std::make_shared<CLASS>(); } \
virtual std::shared_ptr<BASECLASS> make_instance(YAML::Node& node) { return std::make_shared<CLASS>(node.as<CLASS>()); } \
virtual YAML::Node serialize();


#define REGISTER_CLASS(CLASS) \
bool createID##CLASS() { oaSerializer::getTypeMap().insert( {std::string(#CLASS), new CLASS()} ); return true; } \
static bool classID##CLASS = createID##CLASS(); \
YAML::Node CLASS::serialize() { return YAML::convert<CLASS>().encode(*this); }

class oaSerializer {
public:
	static bool reciveScene(const char* filename);
	static bool emitObjects(const char* filename);

	static std::map<std::string, oaObject*>& getTypeMap();
};
