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
	void update(GameObject&);

private:
	GameObject* createPoo();
	std::vector<GameObject*> poo;
};

