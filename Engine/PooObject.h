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
	
	Vec2 tandemDir;
	Vec2 move;
public://functions
	PooObject(PooInputComponent* input, PooPhysicsComponent*, PooGraphicsComponent*, PooCollisionComponent*);
	virtual void update(Keyboard&, float);		//input
	virtual void update(Graphics&, float);		//physics
	virtual void update(GameObject&);			//collision
	virtual void update(DirectX::SpriteBatch&);	//graphics

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

