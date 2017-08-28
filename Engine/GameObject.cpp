#include "GameObject.h"



GameObject::GameObject(ComponentInput* input, ComponentPhysics* physics, ComponentGraphics* graphics, ComponentCollision* collision)
	:input(input),
	physics(physics),
	graphics(graphics),
	collision(collision)
{}
void GameObject::update(GameObject& obj_Active)
{
	collision->update(obj_Active, *this);
}
void GameObject::update(Keyboard& kbd, float delta)
{
	input->update(*this, kbd, delta);
}
void GameObject::update(Graphics& gfx, float delta)
{
	physics->update(*this, delta, gfx);
}
void GameObject::update(DirectX::SpriteBatch& batch)
{
	graphics->update(*this, batch);
}
