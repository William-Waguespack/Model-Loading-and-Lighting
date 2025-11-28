#pragma once
#include "NewShader.h"

class Renderable
{
	virtual void Draw(NewShader& shader) = 0;
};

