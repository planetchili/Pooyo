#pragma once

#include "Graphics.h"
#include "Keyboard.h"


class GameObject;

class ComponentInput
{
public:
	virtual ~ComponentInput() {}
	virtual void update(GameObject&, Keyboard&, float) = 0;
};
class ComponentPhysics
{
public:
	enum eCollides
	{
		TOP,
		LEFT,
		RIGHT,
		BOT,
		BOUNDS_LEFT,
		BOUNDS_RIGHT,
		BOUNDS_BOT,
		DFLT
	}collidesType;
public:
	virtual ~ComponentPhysics() {}
	virtual void movement(GameObject&, float) = 0;
	virtual void collisionBounds(GameObject&, float, float) = 0;
	virtual void collisionObj(GameObject&, GameObject&) = 0;
	virtual void resolveObjCollision(GameObject&, GameObject&) = 0;
	virtual void resolveBoundsCollision(GameObject&, float, float) = 0;
};
class ComponentGraphics
{
public:
	virtual ~ComponentGraphics() {}
	virtual void update(GameObject&, DirectX::SpriteBatch&) = 0;
};


