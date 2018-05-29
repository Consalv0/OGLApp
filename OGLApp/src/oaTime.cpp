
#include "oaMain.h"
#include "oaTime.h"

#include <Windows.h>
#include <ctime>
#include <time.h>

unsigned long oaTime::_lastUpdateTime = 0;
unsigned long oaTime::_lastDeltaTime = 0;

void oaTime::update() {
	_lastDeltaTime = getAppTime() - _lastUpdateTime;
	_lastUpdateTime = getAppTime();
}

float oaTime::getDeltaTime() {
	return _lastDeltaTime / 1000.0F;
}

long oaTime::getDeltaTimeMilis() {
	return _lastDeltaTime;
}

unsigned long oaTime::getAppTime() {
	SYSTEMTIME time;
	GetSystemTime(&time);
	using namespace std;
	return time.wHour * (long)3600000 +
		     time.wMinute * (long)60000 +
		     time.wSecond  * (long)1000 +
		     time.wMilliseconds;
}

