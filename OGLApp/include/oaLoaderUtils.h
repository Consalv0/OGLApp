#pragma once

#include "oaMain.h"

size_t oaFindExtIdx(const char* fileName) {
	size_t len = strlen(fileName);
	size_t idx = len - 1;
	for (size_t i = 0; *(fileName + i); i++) {
		if (*(fileName + i) == '.') {
			idx = i;
		} else if (*(fileName + i) == '/' || *(fileName + i) == '\\') {
			idx = len - 1;
		}
	}
	return idx + 1;
}

std::string getFileExt(const char* fileName) {
	return std::string(fileName).substr(oaFindExtIdx(fileName));
}