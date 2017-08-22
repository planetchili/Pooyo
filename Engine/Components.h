#pragma once

#include "Graphics.h"
#include "GameObject.h"

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
	virtual void update(GameObject&) = 0;
};

