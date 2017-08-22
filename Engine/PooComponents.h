#pragma once

#include "Components.h"


class PooInputComponent : public ComponentInput
{
public:
	virtual void update(GameObject&);
};
class PooPhysicsComponent : public ComponentPhysics
{
public:
	virtual void update(GameObject&);
};

class PooGraphicsComponent : public ComponentGraphics
{
public:
	virtual void update(GameObject&);
};
