#include "GameObject.h"



GameObject::GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics)
	:input(input),
	physics(physics),
	graphics(graphics)
{}

void GameObject::update(Keyboard& kbd)
{
	input->update(*this, kbd);
}
void GameObject::update(Graphics& gfx, float delta)
{
	
	physics->update(*this, delta, gfx);
	graphics->update(*this, gfx);
}
