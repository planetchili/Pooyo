#pragma once

#include "Components.h"

class GameObject
{
public:

	DirectX::XMFLOAT2 velocity;
	DirectX::XMFLOAT2 position;
	
	GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics);
	void update(Graphics&, float delta);

protected:
	ComponentInput* input;
	ComponentPhysics* physics;
	ComponentGraphics* graphics;
};

