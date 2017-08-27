#include "GameObject.h"



GameObject::GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics, ComponentCollision* collision)
	:input(input),
	physics(physics),
	graphics(graphics),
	collision(collision)
{}
void GameObject::update(GameObject* obj)
{
	collision->update(obj, *this);
}
void GameObject::update(Keyboard& kbd, float delta)
{
	input->update(*this, kbd, delta);
}
void GameObject::update(Graphics& gfx, float delta)
{
	
	physics->update(*this, delta, gfx);
	graphics->update(*this, gfx);
}
