#pragma once
#include "Components.h"

class GameObject;

class TandemInptCmpt : public ComponentInput
{
public:
	virtual void update(GameObject&, Keyboard&, float);
};