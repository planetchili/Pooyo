#pragma once


#include <vector>
#include "PooObject.h"
#include "PooComponents.h"
#include "Graphics.h"
#include <chrono>
#include <random>


class PooMachine
{
public:

	PooMachine(Graphics&);
	~PooMachine();

public:

	void update(Graphics&, float delta);
public:
	float midPoint;
	float diameter;
	//rng
	std::mt19937 rng;
	std::uniform_int_distribution<int> distribution;
private:
	
	std::vector<PooObject*> poo;
	PooObject* spawnPoo(DirectX::XMFLOAT2 velocity, DirectX::XMFLOAT2 position);
	Sprite* getSprite(PooObject::eColour colour);
	
public:
	Sprite poo_blue;
	Sprite poo_green;
	Sprite poo_purple;
	Sprite poo_red;
};

