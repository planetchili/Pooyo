#pragma once

#include "Graphics.h"

class GameObject;

class ComponentInput
{
public:
	virtual ~ComponentInput() {}
	virtual void update(GameObject&) = 0;
};
class ComponentPhysics
{
public:
	virtual ~ComponentPhysics() {}
	virtual void update(GameObject&) = 0;
};
class ComponentGraphics
{
public:
	virtual ~ComponentGraphics() {}
	virtual void update(GameObject&, Graphics&) = 0;
};

