#pragma once


#include <vector>
#include "GameObject.h"
#include "PooComponents.h"
#include "Graphics.h"


class PooMachine
{
public:

	PooMachine(Graphics&);
	~PooMachine();

public:

	void update(Graphics&, float delta);
	void linkSprites();
public:

private:

	GameObject* createPoo(DirectX::XMFLOAT2 velocity, DirectX::XMFLOAT2 position);
	std::vector<GameObject*> poo;
	
public:
	Sprite poo_blue;
	Sprite poo_green;
	Sprite poo_purple;
	Sprite poo_red;
};

