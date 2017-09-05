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

public://functions
	PooObject(PooPhysicsComponent*, PooGraphicsComponent*);
	virtual void update(Keyboard&, float);			//input
	virtual void update(float, float, float);		//physics
	virtual void update(GameObject&);				//collision
	virtual void update(DirectX::SpriteBatch&);		//graphics

public://variables

	//connected Poo of same type
	PooObject* ptrPooPrev;
	int sequenceNum;//

	bool isfresh;
	bool hasLanded;
	bool hasCollided;
public:
	int connectPoo(PooObject* nextPoo);
};

