#pragma once
#include "GameObject.h"
#include "PooObject.h"
#include "PooComponents.h"

class TandemPooObj//player Controller
{
public:
	PooObject* mainPoo;
	PooObject* partnerPoo;

public:
	TandemPooObj(ComponentInput*);
	void update(Keyboard&, float);//input
	~TandemPooObj();
};

