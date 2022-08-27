#pragma once
namespace VEE {
	class Event;
	class App {
	public:
		virtual void Init() = 0;
		virtual void OnEvent(Event&) = 0;
		virtual void Run() = 0;
		virtual void ShutDown() = 0;
	};
}
