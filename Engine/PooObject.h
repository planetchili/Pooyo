#pragma once
#include "GameObject.h"
#include "PooComponents.h"

class PooObject : public GameObject
{
public:
	enum eColour
	{
		BLUE,
		GREEN,
		PURPLE,
		RED
	}colourType;
	float diameter;
public://functions
	PooObject(PooInputComponent* input, PooPhysicsComponent* physics, PooGraphicsComponent* graphics);
	virtual void update(Keyboard&);
	virtual void update(Graphics&, float delta);

public://variables

	//connected Poo of same type
	PooObject* ptrPooPrev;
	int sequenceNum;//

	//spawning partner poo
	PooObject* spawnPooPartner;

	bool isfresh;
	bool hasLanded;
public:
	int connectPoo(PooObject* nextPoo);
};

