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
	float soloSpeed = 200.0f;

public://ctor 
	PooObject(PooInputComponent*, PooPhysicsComponent*, PooGraphicsComponent*);
public://virtual update funcs
	virtual void update(Keyboard&) override;					//input
	virtual void update(float, float, float) override;		//physics
	virtual void update(GameObject&) override;				//collision
	virtual void draw(DirectX::SpriteBatch&) override;		//graphics
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

