#include "PooMachine.h"

PooMachine::PooMachine()
{
	poo.push_back(createPoo());
	
}

PooMachine::~PooMachine()
{
}
void PooMachine::loadSprites(Graphics& gfx)
{
	poo_blue	= &gfx.MakeSprite(L"../Art/Node_Blue.png", { 0,0,12,12 }, 4.0f, { 16.0f,24.0f });
	poo_green	= &gfx.MakeSprite(L"../Art/Node_Green.png", { 0,0,12,12 }, 4.0f, { 28.0f,24.0f });
	poo_purple	= &gfx.MakeSprite(L"../Art/Node_Purple.png", { 0,0,12,12 }, 4.0f, { 40.0f,24.0f });
	poo_red		= &gfx.MakeSprite(L"../Art/Node_Red.png", { 0,0,12,12 }, 4.0f, { 52.0f,24.0f });

	reinterpret_cast<PooGraphicsComponent*>(poo[0]->graphics)->spritePoo = poo_blue;
}
void PooMachine::update(DirectX::SpriteBatch& sb)
{
	
	poo[0]->update(sb);

}

GameObject* PooMachine::createPoo()
{
	return new GameObject(new PooInputComponent(), new PooPhysicsComponent(), new PooGraphicsComponent());
}
