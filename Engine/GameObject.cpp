#include "GameObject.h"



GameObject::GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics)
	:input(input),
	physics(physics),
	graphics(graphics)
{}
//collision and resolve
void GameObject::update(GameObject& obj_Active)
{
	physics->collisionObj(obj_Active, *this);
	physics->resolveObjCollision(obj_Active, *this);
}
//input
void GameObject::update(Keyboard& kbd, float delta)
{
	input->update(*this, kbd, delta);
}
//movement
void GameObject::update(float screenwidth, float screenHeight, float delta)
{
	physics->movement(*this, delta);
	physics->collisionBounds(*this, screenwidth, screenHeight);
	physics->resolveBoundsCollision(*this, screenwidth, screenHeight);
}
//graphics
void GameObject::update(DirectX::SpriteBatch& batch)
{
	graphics->update(*this, batch);
}
