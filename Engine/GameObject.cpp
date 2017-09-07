#include "GameObject.h"



GameObject::GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics)
	:
	input(input),
	physics(physics),
	graphics(graphics)
{}
//input
void GameObject::update(GameObject& obj, Keyboard& kbd)
{
	if (input != NULL)
		input->update(*this, kbd);
}
//collision and resolve
void GameObject::update(GameObject& obj_Active)
{
	if (physics != NULL)
	{
		physics->collisionObj(obj_Active, *this);
		physics->resolveObjCollision(obj_Active, *this);
	}
}
//movement
void GameObject::update(float screenwidth, float screenHeight, float delta)
{
	if (physics != NULL)
	{
		physics->movement(*this, delta);
		physics->collisionBounds(*this, screenwidth, screenHeight);
		physics->resolveBoundsCollision(*this, screenwidth, screenHeight);
	}
}
//graphics
void GameObject::draw(DirectX::SpriteBatch& batch)
{
	graphics->draw(*this, batch);
}
