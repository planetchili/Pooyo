#include "GameObject.h"



GameObject::GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics)
	:input(input),
	physics(physics),
	graphics(graphics)
{}


void GameObject::update(Graphics& gfx, float delta)
{
	input->update(*this);
	physics->update(*this, delta, gfx);
	graphics->update(*this, gfx);
}
