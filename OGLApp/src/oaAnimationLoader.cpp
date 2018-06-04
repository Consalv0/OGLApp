
#include "oaLoaderUtils.h"
#include <math.h>
#include <codecvt>
#include <locale>
#include "oaJoint.h"
#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_iterators.hpp"
#include "rapidxml\rapidxml_print.hpp"
#include "rapidxml\rapidxml_utils.hpp"
#include "oaAnimationLoader.h"

std::unordered_map<std::string, std::vector<oaAnimation>> oaAnimationLoader::animationIDs = std::unordered_map<std::string, std::vector<oaAnimation>>();

std::vector<oaAnimation>* oaAnimationLoader::loadAnimation(const char * filePath) {
	if (animationIDs.find(filePath) != animationIDs.end()) {
		return &animationIDs.at(filePath);
	}

	std::string fileExt = oaGetFileExtension(filePath);
	std::vector<oaAnimation>* animation;

	// OBJ parser
	if (fileExt == "dae") {

		animation = loadDAE(
			filePath
		);

		// Format not supported
	} else {
		printf("File extension no supported: '%s'", fileExt.c_str());
		return NULL;
	}

	// Format supported, file unsopported
	if (animation == NULL || animation->empty()) {
		printf("Unable to load '%s'\n", filePath);
		return NULL;
	}

	// All right, add to the list of meshes
	animationIDs.insert({ std::string(filePath), *animation });

	printf("Animation loaded : %s\n", filePath);
	return &animationIDs.find(filePath)->second;
}

std::vector<oaAnimation>* oaAnimationLoader::loadDAE(const char * filePath) {
	using namespace rapidxml;
	std::vector<oaAnimation>* animations = new std::vector<oaAnimation>();
	xml_document<wchar_t> doc;

	// Open file
	file<wchar_t> fileXml(filePath);
	doc.parse<0>(fileXml.data());    // 0 means default parse flags
	xml_node<wchar_t> *collada = doc.first_node(L"COLLADA");
	xml_node<wchar_t> *up_axis = collada->first_node(L"asset")->first_node(L"up_axis");
	// Get the model orientation, OpenGl is Y-Up oriented
	glm::mat4 orientation = glm::mat4();
	if (up_axis) {
		if (wcscmp(up_axis->value(), L"Z_UP") == 0) {
			orientation = { 1,  0, 0, 0,
				0,  0, 1, 0,
				0, -1, 0, 0,
				0,  0, 0, 1 };
		}
		//else if (wcscmp(up_axis->value(), L"Y_UP") == 0) {
		//	orientation = { 1, 0, 0,
		//		              0, 1, 0,
		//		              0, 0, 1, };
		//}
		else if (wcscmp(up_axis->value(), L"X_UP") == 0) {
			orientation = { 0, -1, 0, 0,
				1,  0, 0, 0,
				0,  0, 1, 0,
				0,  0, 0, 1 };
		}
	}

	xml_node<wchar_t> *library_animations = collada->first_node(L"library_animations");
	xml_node<wchar_t> *animation = library_animations->first_node(L"animation");

	while (animation) {
		xml_node<wchar_t> *source = animation->first_node(L"source");
		oaAnimation currentAnimation;
		while (source) {
			if (auto technique_common = source->first_node(L"technique_common")) {
				if (auto accessor = technique_common->first_node(L"accessor")) {
					wchar_t* type = accessor->first_node(L"param")->first_attribute(L"name")->value();
					if (wcscmp(L"TIME", type) == 0) {
						int keys = _wtoi(accessor->first_attribute(L"count")->value()); 
						currentAnimation.size = keys;
						currentAnimation.keyTimes = new float[keys];
						xml_node<wchar_t> *farray = source->first_node(L"float_array");

						std::wistringstream iss(farray->value());
						std::wstring s;
						for (int i = 0; i < keys; i++) {
							s.clear(); iss >> s;
							currentAnimation.keyTimes[i] = (float)_wtof(s.c_str());
						}
					}

					if (wcscmp(L"TRANSFORM", type) == 0) {
						int keys = _wtoi(accessor->first_attribute(L"count")->value());
						currentAnimation.transforms = new oaAnimation::JointTransforms[keys];
						xml_node<wchar_t> *farray = source->first_node(L"float_array");

						std::wistringstream iss(farray->value());
						std::wstring s;
						for (int i = 0; i < keys; i++) {
							glm::mat4 transform;
							for (int j = 0; j < 4; j++) {
								s.clear(); iss >> s;
								transform[j][0] = (float)_wtof(s.c_str());
								s.clear(); iss >> s;
								transform[j][1] = (float)_wtof(s.c_str());
								s.clear(); iss >> s;
								transform[j][2] = (float)_wtof(s.c_str());
								s.clear(); iss >> s;
								transform[j][3] = (float)_wtof(s.c_str());
							}
							currentAnimation.transforms[i] = oaAnimation::JointTransforms(transform);
						}
					}
				}
			}
			source = source->next_sibling(L"source");
		}

		if (auto channel = animation->first_node(L"channel")) {
			if (auto target = channel->first_attribute(L"target")) {
				currentAnimation.name = target->value();
			}
		}

		animations->push_back(currentAnimation);
		animation = animation->next_sibling(L"animation");
	}

	if (!animations || animations->empty()) {
		delete animations;
		return NULL;
	}

	return animations;
}
