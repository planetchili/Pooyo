#include "TandemPooPlCntrlr.h"



TandemPooPlCntrlr::TandemPooPlCntrlr()
	:
	mainPoo(new PooObject(new PooGraphicsComponent)),
	partnerPoo(new PooObject(new PooGraphicsComponent))
{
	
}


TandemPooPlCntrlr::~TandemPooPlCntrlr()
{

}
//input
void TandemPooPlCntrlr::update(Keyboard&, float)
{

}
//physics
void TandemPooPlCntrlr::update(float screenWidth, float screenHeight, float delta)		
{

}
//collision
void TandemPooPlCntrlr::update(GameObject&)				
{

}
//graphics
void TandemPooPlCntrlr::draw(DirectX::SpriteBatch& batch)
{
	mainPoo->draw(batch);
	partnerPoo->draw(batch);
}