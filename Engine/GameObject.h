#pragma once

#include "Components.h"

class GameObject
{
public:

	DirectX::XMFLOAT2 velocity;
	DirectX::XMFLOAT2 position;
	
	GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics);
	virtual void update(Keyboard&);
	virtual void update(Graphics&, float delta);
	
public:
	ComponentInput* input;
	ComponentPhysics* physics;
	ComponentGraphics* graphics;
};

