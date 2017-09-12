#pragma once

#include "Graphics.h"
#include "Keyboard.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class GameObject;
//input
class ComponentInput
{
public:
	virtual ~ComponentInput() {}
	virtual void update(GameObject&, Keyboard&) = 0;
};
//physics
class ComponentPhysics
{
public:
	enum eCollides
	{
		LEFT,
		RIGHT,
		BOT,
		BOUNDS_LEFT,
		BOUNDS_RIGHT,
		BOUNDS_BOT,
		DFLT
	}collidesType;
	Vector2 move = Vector2( 0.0f, 1.0f );
public:
	virtual ~ComponentPhysics() {}
	virtual void movement(GameObject&, float) = 0;
	virtual void collisionBounds(GameObject&, float, float) = 0;
	virtual void collisionObj(GameObject&, GameObject&) = 0;
	virtual void resolveObjCollision(GameObject&, GameObject&) = 0;
	virtual void resolveBoundsCollision(GameObject&, float, float) = 0;
};
//graphics
class ComponentGraphics
{
public:
	virtual ~ComponentGraphics() {}
	virtual void draw(GameObject&, DirectX::SpriteBatch&) = 0;
};


