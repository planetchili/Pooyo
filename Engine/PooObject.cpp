#include "PooObject.h"



PooObject::PooObject(PooInputComponent* input, PooPhysicsComponent* physics, PooGraphicsComponent* graphics)
	: 
	GameObject(input, physics, graphics),
	ptrNextPoo(NULL),
	ptrHeadPoo(NULL),
	sequenceNum(1),
	hasLanded(false),
	hasCollided(false)
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
//connecting pooyo
void PooObject::connectPoo(PooObject* adjacentPoo)
{
	if (adjacentPoo->colourType == this->colourType)
	{
		PooObject* adjacentPooLast = adjacentPoo->getLastPoo();
		if (adjacentPooLast != this)
		{
			if (this->sequenceNum > adjacentPooLast->sequenceNum)
			{
				this->getLastPoo()->ptrNextPoo = adjacentPoo;
				adjacentPoo->updateSeqNum(this->getLastPoo()->sequenceNum + 1);
			}
			else
			{
				adjacentPooLast->ptrNextPoo = this;
				this->updateSeqNum(adjacentPooLast->sequenceNum + 1);
			}
			
			
			//this->hasLanded = true;
			if (adjacentPooLast->ptrHeadPoo == NULL)
				this->ptrHeadPoo = adjacentPooLast;
			else
				this->ptrHeadPoo = adjacentPooLast->ptrHeadPoo;
		}
	}
}
void PooObject::updateSeqNum(int newSeqnum)
{
	this->sequenceNum = newSeqnum;
	if (this->ptrNextPoo != NULL)
		this->ptrNextPoo->updateSeqNum(newSeqnum + 1);
}

