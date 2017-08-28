#pragma once

#include "Components.h"

class GameObject;

class PooInputComponent : public ComponentInput
{
public:
	virtual void update(GameObject&, Keyboard&, float);
};
class PooPhysicsComponent : public ComponentPhysics
{
public:
	virtual void update(GameObject&, float delta, Graphics& gfx);
};

class PooGraphicsComponent : public ComponentGraphics
{
public:
	virtual void update(GameObject&, DirectX::SpriteBatch&);
	Sprite* spritePoo;
};
class PooCollisionComponent: public ComponentCollision
{
public:
	virtual void update(GameObject&, GameObject&);
};
