#pragma once
#include "Window.h"



class Game
{
public:
	Game();
	void Run();
private:

	Window m_GameWindow;
	float deltaTime;

	
};

