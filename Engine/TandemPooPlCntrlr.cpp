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
	GameObject::update(*this, kbd);
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
	if (mainPoo->hasLanded)
	{
		partnerPoo->position = mainPoo->position + tandemDir * diameter;
	}
	else
	{
		partnerPoo->physics->collisionObj(*mainPoo, obj);
		partnerPoo->physics->resolveObjCollision(*mainPoo, obj);
	}
	
	//GameObject::update(obj);
}
//graphics
void TandemPooPlCntrlr::draw(DirectX::SpriteBatch& batch)
{
	mainPoo->GameObject::draw(batch);
	partnerPoo->GameObject::draw(batch);
}

//helper functions
void TandemPooPlCntrlr::reset()
{
	this->multi = -1.0f;
	float radians = rot * multi;

	this->tandemDir = Vector2(cos(radians), sin(radians));
}