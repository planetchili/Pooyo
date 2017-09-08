#pragma once
#include "GameObject.h"
#include "PooObject.h"
#include "TandemComponents.h"


class TandemPooPlCntrlr : public GameObject //player Controller
{
public:
	PooObject* mainPoo;
	PooObject* partnerPoo;
	float rot;
	float multi;
	Vector2 tandemDir;
	

public:
	TandemPooPlCntrlr(TandemInptCmpt*, TandemPhysicsCmpt*, TandemGraphicsCmpt*);
	virtual void update(Keyboard&);					//input
	virtual void update(float, float, float);		//physics
	virtual void update(GameObject&);				//collision
	virtual void draw(DirectX::SpriteBatch&);		//graphics
	
};

