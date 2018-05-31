#pragma once

#include "oaMain.h"
#include <fstream>

static std::string oaGetFileExtension(const char* fileName) {
	size_t len = strlen(fileName);
	size_t idx = len - 1;
	for (size_t i = 0; *(fileName + i); i++) {
		if (*(fileName + i) == '.') {
			idx = i;
		} else if (*(fileName + i) == '/' || *(fileName + i) == '\\') {
			idx = len - 1;
		}
	}

	idx += 1;

	return std::string(fileName).substr(idx);
}

static int oaEndsWidth(const char* withwhat, const char* what) {
	int l1 = strlen(withwhat);
	int l2 = strlen(what);
	if (l1 > l2)
		return 0;

	return strcmp(withwhat, what + (l2 - l1)) == 0;
}