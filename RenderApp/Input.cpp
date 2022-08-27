#include "Renderpch.h"
#include "Input.h"
#include "Window.h"
#include <GLFW/glfw3.h>

namespace VEE {
	int Input::m_LastState = 0;
	bool Input::IsKeyReleased(Window* window, int keycode)
	{
		auto state = glfwGetKey(window->GetGLFWWindow(), keycode);
		if (m_LastState == GLFW_PRESS || m_LastState == GLFW_REPEAT) {
			m_LastState = state;
			return state == GLFW_RELEASE;
		}
		m_LastState = state;
		return false;
	}

	bool Input::IsKeyPressed(Window* window, int keycode) {
		
		auto state = glfwGetKey(window->GetGLFWWindow(), keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(Window* window, int button) {

		auto state = glfwGetMouseButton(window->GetGLFWWindow(), button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition(Window* window) {
		double xpos, ypos;
		glfwGetCursorPos(window->GetGLFWWindow(), &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX(Window* window) {
		auto [x, y] = GetMousePosition(window);
		return x;

	}
	float Input::GetMouseY(Window* window) {
		auto [x, y] = GetMousePosition(window);
		return y;
	}
}
