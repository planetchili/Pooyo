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
	float soloSpeed = 150.0f;

public://ctor 
	PooObject(PooInputComponent*, PooPhysicsComponent*, PooGraphicsComponent*);
public://update funcs
	virtual void update(Keyboard&);					//input
	virtual void update(float, float, float);		//physics
	virtual void update(GameObject&);				//collision
	virtual void draw(DirectX::SpriteBatch&);		//graphics
public://member funcs
	PooObject* getLastPoo();
	void updateSeqNum(int);
	void updaatHeadPtr(PooObject*);
	void connectPoo(PooObject*);
	
public://member variables

	//connected Poo of same type
	int sequenceNum;
	PooObject* ptrHeadPoo;
	PooObject* ptrNextPoo;
	

	//collision
	bool hasCollided;
	//stacked or onground
	bool hasLanded;
};

