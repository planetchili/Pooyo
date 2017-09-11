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
	bool active = true;
	bool connected = true;

public://main functions
	TandemPooPlCntrlr(TandemInptCmpt*, TandemPhysicsCmpt*, TandemGraphicsCmpt*);
	virtual void update(Keyboard&);					//input
	virtual void update(float, float, float);		//physics
	virtual void update(GameObject&);				//collision
	virtual void draw(DirectX::SpriteBatch&);		//graphics
public://helper functiona
	void reset();
	void updateTandem(PooObject*, PooObject*, float polarity = 1.0f);
	
};

