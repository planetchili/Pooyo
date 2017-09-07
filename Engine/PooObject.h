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
	
	Vector2 move;

public://ctor 
	PooObject(PooGraphicsComponent*);
public://update funcs
	virtual void update(float, float, float);		//physics
	virtual void update(GameObject&);				//collision
	virtual void draw(DirectX::SpriteBatch&);		//graphics
public://member funcs
	PooObject* getLastPoo();
	void updateSeqNum(int newSeqnum);
public://member variables

	//connected Poo of same type
	PooObject* ptrNextPoo;

	int sequenceNum;//

	bool hasLanded;
public:
	int connectPoo(PooObject* nextPoo);
};

