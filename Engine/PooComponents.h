#pragma once

#include "Components.h"

class PooInputComponent : public ComponentInput
{
public:
	virtual void update();
};
class PooPhysicsComponent : public ComponentPhysics
{
public:
	virtual void update();
};

class PooGraphicsComponent : public ComponentGraphics
{
public:
	virtual void update();
};
