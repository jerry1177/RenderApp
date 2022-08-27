// RenderApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "Renderpch.h"
#include "GraphicsApp.h"
int main()
{
    VEE::App* app = new VEE::GraphicsApp();
    app->Init();
    app->Run();
    app->ShutDown();
    delete app;
}
