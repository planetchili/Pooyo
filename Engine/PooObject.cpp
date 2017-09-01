#include "PooObject.h"



PooObject::PooObject(PooInputComponent* input, PooPhysicsComponent* physics, PooGraphicsComponent* graphics)
	: 
	GameObject(input, physics, graphics),
	ptrPooPrev(NULL),
	sequenceNum(1),
	isfresh(true),
	hasLanded(false),
	hasCollided(false),
	tandemDir(0.0f, 0.0f),
	move(0.0f, 0.0f)
{

}
//object collision
void PooObject::update(GameObject& obj_Active)
{
	GameObject::update(obj_Active);

}
//input
void PooObject::update(Keyboard& kbd, float delta)
{
	GameObject::update(kbd, delta);
}
//physics
void PooObject::update(float screenWidth, float screenHeight, float delta)
{
	GameObject::update(screenWidth, screenHeight, delta);
}
//graphics
void PooObject::update(DirectX::SpriteBatch& batch)
{
	GameObject::update(batch);
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
