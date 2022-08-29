#pragma once
namespace VEE {
	class Window;
	class Input
	{
	public:
		static bool IsKeyReleased(Window* window, int keycode);
		static bool IsKeyPressed(Window* window, int keycode);
		static std::pair<float, float> GetMousePosition(Window* window);
		static bool IsMouseButtonPressed(Window* window, int button);
		static float GetMouseX(Window* window);
		static float GetMouseY(Window* window);
	private:
		static int m_LastState;
	};
}

