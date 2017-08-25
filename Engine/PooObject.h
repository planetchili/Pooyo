#pragma once
#include "GameObject.h"
class PooObject : public GameObject
{
public://functions
	PooObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics);
	~PooObject();

public://variables

	//connected Poo of same type
	PooObject* ptrPooLeft;
	PooObject* ptrPooRight;
	PooObject* ptrPooUp;
	PooObject* ptrPooDown;

	//spawning partner poo
	PooObject* spawnPooPartner;

	bool Isfresh;
	bool HasLanded;

};

