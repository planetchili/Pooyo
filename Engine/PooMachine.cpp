#include "PooMachine.h"



PooMachine::PooMachine()
{
	poo.push_back(createPoo());
}


PooMachine::~PooMachine()
{
}

void PooMachine::update(GameObject& obj)
{
	for (auto p : poo)
	{
		p->update(obj);
	}
}

GameObject* PooMachine::createPoo()
{
	return new GameObject(new PooInputComponent(), new PooPhysicsComponent(), new PooGraphicsComponent());
}
