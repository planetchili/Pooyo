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
	struct Dir
	{
		float x = 0.0f;
		float y = 0.0f;
	}tandemDir;

public://functions
	PooObject(PooInputComponent* input, PooPhysicsComponent*, PooGraphicsComponent*, PooCollisionComponent*);
	virtual void update(GameObject*);
	virtual void update(Keyboard&, float);
	virtual void update(Graphics&, float);

public://variables

	//connected Poo of same type
	PooObject* ptrPooPrev;
	int sequenceNum;//

	//spawning partner poo
	PooObject* ptrTandem;

	bool isfresh;
	bool hasLanded;
	bool hasCollided;
public:
	int connectPoo(PooObject* nextPoo);
};

