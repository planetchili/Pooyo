#pragma once

#include "Components.h"

class GameObject;
//Input
class PooInputComponent : public ComponentInput
{
public:
	virtual void update(GameObject&, Keyboard&, float);
};
//Physics
class PooPhysicsComponent : public ComponentPhysics
{

public:
	virtual void movement(GameObject&, float delta);
	virtual void collisionBounds(GameObject&, float, float);
	virtual void collisionObj(GameObject&, GameObject&);
	virtual void resolveObjCollision(GameObject&, GameObject&);
	virtual void resolveBoundsCollision(GameObject&, float, float);
};
//Graphics
class PooGraphicsComponent : public ComponentGraphics
{
public:
	virtual void draw(GameObject&, DirectX::SpriteBatch&);
	Sprite* spritePoo;
};

