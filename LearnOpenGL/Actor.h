#pragma once
#include "Renderable.h"
#include "memory"
#include <vector>

//Add components like mesh compoenents which is where renderables is stored

class Actor
{
public: 
	Actor();

private:
	std::vector<std::unique_ptr<Renderable>> renderables;
};

