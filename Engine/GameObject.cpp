#include "GameObject.h"



GameObject::GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics)
	:
	input(input),
	physics(physics),
	graphics(graphics)
{}
//input
void GameObject::update(Keyboard& kbd)
{
	if (input != NULL)
		input->update(*this, kbd);
}
//physics
void GameObject::update(float screenwidth, float screenHeight, float delta)
{
	if (physics != NULL)
	{
		physics->movement(*this, delta);
		physics->collisionBounds(*this, screenwidth, screenHeight);
		physics->resolveBoundsCollision(*this, screenwidth, screenHeight);
	}
}
//collision and resolve
void GameObject::update(GameObject& obj_InActive)
{
	if (physics != NULL)
	{
		physics->collisionObj(*this, obj_InActive);
		physics->resolveObjCollision(*this, obj_InActive);
	}
}
//graphics
void GameObject::draw(DirectX::SpriteBatch& batch)
{
	graphics->draw(*this, batch);
}
