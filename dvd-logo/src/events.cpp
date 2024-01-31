#include "events.h"

bool events::isMouseActive = false;
float events::xyCusorPos[2] = { 0, 0 };
float events::lastXYCusorPos[2] = { 0, 0 };

float lastX = 1000.0f / 2.0f;
float lastY = 1000.0f / 2.0f;
bool firstMouse = true;


static void setCusorPos(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	events::xyCusorPos[0] = xoffset;
	events::xyCusorPos[1] = yoffset;
}




events::events(GLFWwindow* window)
{
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)setCusorPos);
}


bool events::mouseMoved()
{
	if (events::lastXYCusorPos[0] != events::xyCusorPos[0])
	{
		events::lastXYCusorPos[0] = events::xyCusorPos[0];
		events::lastXYCusorPos[1] = events::xyCusorPos[1];

		return true;
	}
	else
	{
		return false;
	}
}

float const* events::getCusorPosition()
{
	return xyCusorPos;
}
