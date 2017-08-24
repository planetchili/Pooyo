#include "PooMachine.h"

PooMachine::PooMachine(Graphics& gfx)
	: 
	poo_blue(gfx.MakeSprite(L"../Art/Node_Blue.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_green(gfx.MakeSprite(L"../Art/Node_Green.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_purple(gfx.MakeSprite(L"../Art/Node_Purple.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_red(gfx.MakeSprite(L"../Art/Node_Red.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f }))

{
	float midPoint = gfx.ScreenWidth / 2.0f;
	float equidistant = 2 * 12 * 4;

	for (int i = 0; i < 4; i++)
	{
		poo.push_back(createPoo(
			DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(midPoint - equidistant + (float)i * 12.0f * 4, -12.0f  * 4)));
	}
}

PooMachine::~PooMachine()
{
	
}
void PooMachine::linkSprites()
{
	reinterpret_cast<PooGraphicsComponent*>(poo[0]->graphics)->spritePoo = &poo_blue;
	reinterpret_cast<PooGraphicsComponent*>(poo[1]->graphics)->spritePoo = &poo_green;
	reinterpret_cast<PooGraphicsComponent*>(poo[2]->graphics)->spritePoo = &poo_purple;
	reinterpret_cast<PooGraphicsComponent*>(poo[3]->graphics)->spritePoo = &poo_red;
}
void PooMachine::update(Graphics& gfx, float delta)
{
	for (auto p : poo)
	{
		p->update(gfx, delta);
	}
	
}

GameObject* PooMachine::createPoo(DirectX::XMFLOAT2 velocity, DirectX::XMFLOAT2 position)
{
	GameObject* gameobject = new GameObject(new PooInputComponent(), new PooPhysicsComponent(), new PooGraphicsComponent());
	gameobject->velocity = velocity;
	gameobject->position = position;

	return gameobject;
}
