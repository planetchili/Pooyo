#pragma once

#include "Components.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;
class GameObject
{
public:
	float diameter = 12.0f * 4.0f;
	float speed = 20.0f;
	Vector2 position;

	GameObject(ComponentInput*, ComponentPhysics*, ComponentGraphics*);
	virtual void update(Keyboard&, float);//input
	virtual void update(Graphics&, float);//physics
	virtual void update(GameObject&);//collision
	virtual void update(DirectX::SpriteBatch&);//graphics
	
public:
	ComponentInput* input;
	ComponentPhysics* physics;
	ComponentGraphics* graphics;
};

