#pragma once
#include "error.h"

class events
{
public:
	events() = default;

	events(GLFWwindow* window);

	~events() = default;

	bool mouseMoved();

	float const* getCusorPosition();

	public:
	static bool isMouseActive;
	static float xyCusorPos[2];
	static float lastXYCusorPos[2];
};


