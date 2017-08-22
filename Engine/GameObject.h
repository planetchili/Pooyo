#pragma once

#include "Components.h"

class GameObject
{
public:

	GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics);
	void update();

private:
	ComponentInput* input;
	ComponentPhysics* physics;
	ComponentGraphics* graphics;
};

