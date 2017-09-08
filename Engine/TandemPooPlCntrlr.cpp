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
}
//graphics
void TandemPooPlCntrlr::draw(DirectX::SpriteBatch& batch)
{
	mainPoo->GameObject::draw(batch);
	partnerPoo->GameObject::draw(batch);
}