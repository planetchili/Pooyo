#pragma once
//#include <d3d12.h>
//#include <SimpleMath.h>

#include "Components.h"



class GameObject
{
public:
	//Vector2 velocity;
	//Vector2 position;

	GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics);
	void update(GameObject&);

private:
	ComponentInput* input;
	ComponentPhysics* physics;
	ComponentGraphics* graphics;

};

