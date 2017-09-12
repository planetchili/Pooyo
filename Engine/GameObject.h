#pragma once

#include "Components.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;
class GameObject
{
public:
	float diameter = 12.0f * 4.0f;	//  pixelsize scaled by 4 times
	float speed = 20.0f;			//	20 pixels per second? fall rate
	Vector2 position;

	GameObject(ComponentInput*, ComponentPhysics*, ComponentGraphics*);
	virtual void update(GameObject&, Keyboard&);	//input
	virtual void update(float, float, float);		//physics
	virtual void update(GameObject&);				//collision
	virtual void draw(DirectX::SpriteBatch&);		//graphics
	
public:
	ComponentInput* input;
	ComponentPhysics* physics;
	ComponentGraphics* graphics;
};

