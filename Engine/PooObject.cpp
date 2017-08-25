#include "PooObject.h"



PooObject::PooObject(PooInputComponent* input, PooPhysicsComponent* physics, PooGraphicsComponent* graphics)
	: GameObject(input, physics, graphics),
	ptrPooPrev(NULL),
	sequenceNum(1),
	isfresh(true),
	hasLanded(false)
{

}

void PooObject::update(Graphics& gfx, float delta)
{
	GameObject::update(gfx, delta);
}
int PooObject::connectPoo(PooObject* prevPoo)
{
	if (this->colourType == prevPoo->colourType)
	{
		this->ptrPooPrev = prevPoo;
		this->sequenceNum += ptrPooPrev->sequenceNum;
		this->isfresh = false;
		this->hasLanded = true;
		this->colourType = BLUE;
		return sequenceNum;
	}
	return 0;
}
