#pragma once


#include <vector>
#include <deque>
#include <list>
#include "PooObject.h"
#include "TandemPooPlCntrlr.h"
#include "PooComponents.h"
#include "Graphics.h"
#include "Keyboard.h"
#include <chrono>
#include <random>


class PooMachine
{
public:

	PooMachine(Graphics&);
	~PooMachine();
	enum eMachineState
	{
		SPAWN,
		PLAY,
		LIMBO,
		CONNECT,
		FREEFALL,
		DFLT
	}state = DFLT;
public:

	void update( Graphics&, Keyboard&, float delta);
	void update_GFX(Graphics&);
	void update_collision();
public:
	float midPoint;
	float diameter;
	//rng
	std::mt19937 rng;
	std::uniform_int_distribution<int> distribution;
private:
	
	std::vector<std::deque<PooObject*>> pooyo;
	std::queue<PooObject*> checkPoo;

	TandemPooPlCntrlr *tandemPooPlcntrlr;
	void createTandemPooObj(float x, float y);
	void spawnTandemPoo();
	Sprite* getSprite(PooObject::eColour colour);
	void placePooyo(PooObject*);
	void connectPooyo(PooObject*);
	bool checkBounds(const int, const int);
	void removeGroup(PooObject* poo);

public:
	Sprite poo_blue;
	Sprite poo_green;
	Sprite poo_purple;
	Sprite poo_red;
};

