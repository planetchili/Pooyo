#include "TandemPooPlCntrlr.h"
#include "SimpleMath.h"

using namespace DirectX;


TandemPooPlCntrlr::TandemPooPlCntrlr(TandemInptCmpt* input, TandemPhysicsCmpt* physics, TandemGraphicsCmpt* graphics)
	:
	GameObject(input, physics, graphics),
	rot(XM_PIDIV2),//DirectX PI / 2f
	multi(-1.0f),
	tandemDir(0.0f, -1.0f)
{
	
}
//input
void TandemPooPlCntrlr::update(Keyboard& kbd)
{
	GameObject::update(kbd);
}
//physics
void TandemPooPlCntrlr::update(float screenWidth, float screenHeight, float delta)		
{
	GameObject::update(screenWidth, screenHeight, delta);
}
//collision
void TandemPooPlCntrlr::update(GameObject& obj)
{
	GameObject::update(obj);

}
//graphics
void TandemPooPlCntrlr::draw(DirectX::SpriteBatch& batch)
{
	if(mainPoo!= NULL)
		mainPoo->GameObject::draw(batch);
	if(partnerPoo != NULL)
		partnerPoo->GameObject::draw(batch);
}

//helper functions
void TandemPooPlCntrlr::reset()
{
	this->mainPoo = NULL;
	this->partnerPoo = NULL;
	this->state = TandemPooPlCntrlr::eTandemState::ACTIVE;
	this->multi = -1.0f;
	float radians = rot * multi;
	
	this->tandemDir = Vector2((int)(cos(radians)), (int)(sin(radians)));
}
void TandemPooPlCntrlr::updateTandem(PooObject* leader, PooObject* follower, float polarity)
{
	if (follower != NULL)
	{
		follower->position = leader->position + tandemDir * polarity * diameter;
	}
}
void TandemPooPlCntrlr::setCollisionType(PooObject* poo)//move to pooObject
{
	if (this->physics->move.x < 0.0f)
	{
		poo->physics->collidesType = ComponentPhysics::eCollides::LEFT;
	}
	else if (this->physics->move.x > 0.0f)
	{
		poo->physics->collidesType = ComponentPhysics::eCollides::RIGHT;
	}
	else
	{
		poo->physics->collidesType = ComponentPhysics::eCollides::BOT;
	}
}
void TandemPooPlCntrlr::setTandemDir(const float& radians)
{
	int dir_x = (int)(cos(radians));
	int dir_y = (int)(sin(radians));
	this->tandemDir = Vector2((float)dir_x, (float)dir_y);
}