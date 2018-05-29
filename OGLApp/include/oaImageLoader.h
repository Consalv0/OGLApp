#pragma once

#include "oaMain.h"
#include "oaGraphics.h"
#include <unordered_map>

class oaImageLoader {
public:
	static std::unordered_map<std::string, GLuint> imagesIDs;

	static GLuint loadImage(std::string &filePath, size_t &outWidth, size_t &outHeight, bool &outHasAlpha);

private:
	static GLuint loadPNG(const char *filePath, size_t &outWidth, size_t &outHeight);
	static GLuint loadPNG24(const char *filePath, size_t &outWidth, size_t &outHeight);
};