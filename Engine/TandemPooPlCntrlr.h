#pragma once
#include "GameObject.h"
#include "PooObject.h"
#include "TandemComponents.h"

class TandemPooPlCntrlr : public GameObject //player Controller
{
public:
	PooObject* mainPoo;
	PooObject* partnerPoo;

	Vector2 tandemDir = { 0.0f, -1.0f };
	Vector2 move = { 0.0f, 1.0f };

public:
	TandemPooPlCntrlr(TandemInptCmpt*, TandemPhysicsCmpt*, TandemGraphicsCmpt*);
	virtual void update(Keyboard&);					//input
	virtual void update(float, float, float);		//physics
	virtual void update(GameObject&);				//collision
	virtual void draw(DirectX::SpriteBatch&);		//graphics
	
};

