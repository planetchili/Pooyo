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
		PLACE,
		CHAINS,
		FREEFALL,
		CONNECT,
		REMOVE,
		DFLT
	}state = DFLT;
public:
	//main update funtion
	void update( Graphics&, Keyboard&, float);
public:

	float diameter;
	//random number generator
	std::mt19937 rng;
	std::uniform_int_distribution<int> distribution;
private:
	//player controller
	TandemPooPlCntrlr *tandemPooPlcntrlr;

	std::vector<std::deque<PooObject*>> pooyo;
	std::list<PooObject*> checkPoo;
	std::list<int> columns;
	
	//state machine functions
	void play(Graphics&, Keyboard&, float);
	void place();
	void freefall(Graphics&, float);
	void connect();
	void remove();
	void chain(Graphics&, float);

	//update functions
	void update_GFX(Graphics&);
	void update_collision_tandem();
	void update_collision_pooyo(Graphics&, float);

	//pooyo monitoring functions
	void createTandemPooObj(float x, float y);
	void spawnTandemPoo();
	void placePooyo(PooObject*);
	void checkAdjMatchUps();
	void connectPooyo(PooObject*);
	bool checkBounds(const int, const int);
	void removeGroup(PooObject* poo);
	void resetGroup(PooObject* poo);
	void cleanUpPooyo();
	bool columnsHasLanded();
	
	//helper functions
	Sprite* getSprite(PooObject::eColour colour);

public:
	Sprite poo_blue;
	Sprite poo_green;
	Sprite poo_purple;
	Sprite poo_red;
};

