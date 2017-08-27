#pragma once

#include "Graphics.h"
#include "Keyboard.h"

class GameObject;

class ComponentInput
{
public:
	virtual ~ComponentInput() {}
	virtual void update(GameObject&, Keyboard& ) = 0;
};
class ComponentPhysics
{
public:
	virtual ~ComponentPhysics() {}
	virtual void update(GameObject&, float, Graphics&) = 0;
};
class ComponentGraphics
{
public:
	virtual ~ComponentGraphics() {}
	virtual void update(GameObject&, Graphics&) = 0;
};

