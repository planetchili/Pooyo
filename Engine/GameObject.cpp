#include "GameObject.h"



GameObject::GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics)
	:input(input),
	physics(physics),
	graphics(graphics)
{}


void GameObject::update(Graphics& gfx)
{
	input->update(*this);
	physics->update(*this);
	graphics->update(*this, gfx);
}
