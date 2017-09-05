#pragma once
#include "GameObject.h"
#include "PooObject.h"
#include "PooComponents.h"
#include "TandemComponents.h"

class TandemPooPlCntrlr//player Controller
{
public:
	PooObject* mainPoo;
	PooObject* partnerPoo;

	Vector2 tandemDir = { 0.0f, -1.0f };
	Vector2 move = { 0.0f, 1.0f };

	float speed = 20.0f;			//	20 pixels per second? fall rate
	float diameter = 12.0f * 4.0f;	//  12 pixelsize scaled by 4 times

public:
	TandemPooPlCntrlr(TandemInptCmpt*, TandemPhysicsCmpt*, PooGraphicsComponent*);
	virtual void update(Keyboard&, float);			//input
	virtual void update(float, float, float);		//physics
	virtual void update(GameObject&);				//collision
	virtual void update(DirectX::SpriteBatch&);		//graphics
	~TandemPooPlCntrlr();
	
};

