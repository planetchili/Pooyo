#pragma once


#include <vector>
#include "GameObject.h"
#include "PooComponents.h"
#include "Graphics.h"


class PooMachine
{
public:

	PooMachine();
	~PooMachine();

public:

	void update(Graphics&);
	void loadSprites(Graphics& gfx);
public:

private:
	GameObject* createPoo();
	std::vector<GameObject*> poo;
public:
	Sprite* poo_blue;
	Sprite* poo_green;
	Sprite* poo_purple;
	Sprite* poo_red;

};

