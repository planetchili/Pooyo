#pragma once

#include "Components.h"

class GameObject
{
public:
	float diameter = 12.0f * 4.0f;
	float speed = 20.0f;
	DirectX::XMFLOAT2 position;
	
	GameObject(ComponentInput*, ComponentPhysics*, ComponentGraphics*, ComponentCollision*);
	virtual void update(GameObject*);
	virtual void update(Keyboard&, float);
	virtual void update(Graphics&, float);
	
public:
	ComponentInput* input;
	ComponentPhysics* physics;
	ComponentGraphics* graphics;
	ComponentCollision* collision;
};

