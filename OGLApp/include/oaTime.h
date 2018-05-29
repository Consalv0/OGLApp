#pragma once

class oaTime {
public:
	// Update the update time
	// Don't use this unless you know what are you doing
	static void update();
	// Update the fixed update time
	// Don't use this unless you know what are you doing
	// static void fixedUpdate();

	// Time since the last frame;
	static float getDeltaTime();
	static long getDeltaTimeMilis();

	// Time since the app is running;
	static unsigned long getAppTime();

private:
	// Time since the last update callback
	static unsigned long _lastUpdateTime;
	static unsigned long _lastDeltaTime;
};