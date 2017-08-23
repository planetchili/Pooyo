#pragma once

//#include <d3d11.h>

#include <SpriteBatch.h>
#include "Components.h"

class GameObject
{
public:

	DirectX::XMFLOAT2 velocity;
	DirectX::XMFLOAT2 position;

	GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics);
	void update(Graphics&);

private:
	ComponentInput* input;
	ComponentPhysics* physics;
public:
	ComponentGraphics* graphics;
};

