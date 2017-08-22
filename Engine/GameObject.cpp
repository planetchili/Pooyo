#include "GameObject.h"



GameObject::GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics)
	:input(input),
	physics(physics),
	graphics(graphics)
{}


void GameObject::update(GameObject& obj)
{
	input->update(obj);
	physics->update(obj);
	graphics->update(obj);
}
