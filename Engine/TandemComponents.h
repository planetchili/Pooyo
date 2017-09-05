#pragma once
#include "Components.h"

class GameObject;

//Input
class TandemInptCmpt : public ComponentInput
{
public:
	virtual void update(TandemPooPlCntrlr&, Keyboard&, float);
};
//Physics
class TandemPhysicsCmpt : public ComponentPhysics
{

public:
	virtual void movement(TandemPooPlCntrlr&, float delta);
	virtual void collisionBounds(TandemPooPlCntrlr&, float, float);
	virtual void collisionObj(TandemPooPlCntrlr&, GameObject&);
	virtual void resolveObjCollision(TandemPooPlCntrlr&, GameObject&);
	virtual void resolveBoundsCollision(TandemPooPlCntrlr&, float, float);
};