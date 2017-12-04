#pragma once

#include "Components.h"

class GameObject;

//Input
class PooInputComponent : public ComponentInput
{
public:
	virtual void update(GameObject&, Keyboard&);
};
//Physics
class PooPhysicsComponent : public ComponentPhysics
{

public:
	virtual void movement(GameObject&, float delta) override;
	virtual void collisionBounds(GameObject&, float, float) override;
	virtual void collisionObj(GameObject&, GameObject&) override;
	virtual void resolveObjCollision(GameObject&, GameObject&) override;
	virtual void resolveBoundsCollision(GameObject&, float, float) override;
};
//Graphics
class PooGraphicsComponent : public ComponentGraphics
{
public:
	virtual void draw(GameObject&, DirectX::SpriteBatch&) override;
	Sprite* spritePoo;
};

