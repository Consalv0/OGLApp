#pragma once

#include "oaAnimation.h"
#include "oaGraphics.h"
#include <unordered_map>

class oaAnimationLoader {
public:

	static std::unordered_map<std::string, std::vector<oaAnimation>> animationIDs;

	static std::vector<oaAnimation>* loadAnimation(const char *filePath);

private:

	static std::vector<oaAnimation>* loadDAE(const char *filePath);
};