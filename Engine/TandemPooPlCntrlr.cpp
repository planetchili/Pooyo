#include "TandemPooPlCntrlr.h"



TandemPooPlCntrlr::TandemPooPlCntrlr(TandemInptCmpt* input, TandemPhysicsCmpt* physics, TandemGraphicsCmpt* graphics)
	:
	GameObject(input, physics, graphics)
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