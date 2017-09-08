#include "PooObject.h"



PooObject::PooObject(PooInputComponent* input, PooPhysicsComponent* physics, PooGraphicsComponent* graphics)
	: 
	GameObject(input, physics, graphics),
	ptrNextPoo(NULL),
	sequenceNum(1),
	hasLanded(false),
	move(0.0f, 0.0f)
{

}
//input
void PooObject::update(Keyboard& kbd)
{
	GameObject::update(*this, kbd);
}
//object collision
void PooObject::update(GameObject& obj_InActive)
{
	GameObject::update(obj_InActive);
}
//physics
void PooObject::update(float screenWidth, float screenHeight, float delta)
{
	GameObject::update(screenWidth, screenHeight, delta);
}
//graphics
void PooObject::draw(DirectX::SpriteBatch& batch)
{
	GameObject::draw(batch);
}
PooObject* PooObject::getLastPoo()
{
	if (this->ptrNextPoo != NULL)
		return this->ptrNextPoo->getLastPoo();

	return this;
}
void PooObject::updateSeqNum(int newSeqnum)
{
	this->sequenceNum = newSeqnum;
	if (this->ptrNextPoo != NULL)
		this->ptrNextPoo->updateSeqNum(newSeqnum + 1);
}
void PooObject::connectPoo(PooObject* collidiedPoo)
{
	if (this->colourType == collidiedPoo->colourType)
	{
		this->ptrNextPoo = collidiedPoo->getLastPoo();
		this->updateSeqNum(collidiedPoo->sequenceNum + 1);
		this->hasLanded = true;
	}
}
