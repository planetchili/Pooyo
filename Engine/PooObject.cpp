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
	GameObject::update(kbd);
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
			if (this->sequenceNum > 1)
			{
				if (adjacentPooLast->ptrHeadPoo != this->ptrHeadPoo)
				{
					adjacentPooLast->ptrNextPoo = this->ptrHeadPoo;
					this->ptrHeadPoo->updateSeqNum(adjacentPooLast->sequenceNum + 1);
					//this->ptrHeadPoo->updaatHeadPtr(adjacentPooLast->ptrHeadPoo);
					if (adjacentPooLast->ptrHeadPoo == NULL) //?headptr is null in mid sequence
						this->ptrHeadPoo->updaatHeadPtr(adjacentPooLast);
					else
						this->ptrHeadPoo->updaatHeadPtr(adjacentPooLast->ptrHeadPoo);
				}
				
			}
			else
			{
				if (adjacentPooLast->ptrHeadPoo != this)
				{
					adjacentPooLast->ptrNextPoo = this;
					this->updateSeqNum(adjacentPooLast->sequenceNum + 1);
					if (adjacentPooLast->ptrHeadPoo == NULL)
						this->updaatHeadPtr(adjacentPooLast);
					else
						this->updaatHeadPtr(adjacentPooLast->ptrHeadPoo);
				}
			}

			
		}
	}
}
void PooObject::updateSeqNum(int newSeqnum)
{
	this->sequenceNum = newSeqnum;
	if (this->ptrNextPoo != NULL)
		this->ptrNextPoo->updateSeqNum(newSeqnum + 1);
}
void PooObject::updaatHeadPtr(PooObject* headPtr)
{
	this->ptrHeadPoo = headPtr;
	if (this->ptrNextPoo != NULL)
		this->ptrNextPoo->updaatHeadPtr(headPtr);
}
