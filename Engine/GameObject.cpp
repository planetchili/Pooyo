#include "GameObject.h"



GameObject::GameObject(ComponentPhysics* physics, ComponentGraphics* graphics)
	:
	physics(physics),
	graphics(graphics)
{}
//collision and resolve
void GameObject::update(GameObject& obj_Active)
{
	physics->collisionObj(obj_Active, *this);
	physics->resolveObjCollision(obj_Active, *this);
}
//movement
void GameObject::update(float screenwidth, float screenHeight, float delta)
{
	physics->movement(*this, delta);
	physics->collisionBounds(*this, screenwidth, screenHeight);
	physics->resolveBoundsCollision(*this, screenwidth, screenHeight);
}
//graphics
void GameObject::draw(DirectX::SpriteBatch& batch)
{
	graphics->draw(*this, batch);
}
