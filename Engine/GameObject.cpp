#include "GameObject.h"



GameObject::GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics)
	:input(input),
	physics(physics),
	graphics(graphics)
{}


void GameObject::update()
{
	input->update();
	physics->update();
	graphics->update();
}
