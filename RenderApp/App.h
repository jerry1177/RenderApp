#pragma once
class App {
public:
	virtual void Init() = 0;
	virtual void Run() = 0;
	virtual void ShutDown() = 0;
};

