#include "PooObject.h"



PooObject::PooObject(PooGraphicsComponent* graphics)
	: 
	GameObject(NULL, graphics),
	ptrNextPoo(NULL),
	sequenceNum(1),
	hasLanded(false),
	move(0.0f, 0.0f)
{

}
//object collision
void PooObject::update(GameObject& obj_Active)
{
	GameObject::update(obj_Active);
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

}
int PooObject::connectPoo(PooObject* collidiedPoo)
{
	if (this->colourType == collidiedPoo->colourType)
	{
		this->ptrNextPoo = collidiedPoo->getLastPoo();
		this->sequenceNum = ptrPrevPoo->sequenceNum + 1;
		this->hasLanded = true;
		this->colourType = BLUE;
		return sequenceNum;
	}
	return 0;
}
