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
	virtual ~ComponentPhysics() {}
	virtual void update(GameObject&, float, Graphics&) = 0;
	virtual void collision(GameObject&, GameObject&) = 0;
};
class ComponentGraphics
{
public:
	virtual ~ComponentGraphics() {}
	virtual void update(GameObject&, DirectX::SpriteBatch&) = 0;
};


