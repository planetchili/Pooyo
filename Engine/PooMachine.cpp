#include "PooMachine.h"

PooMachine::PooMachine(Graphics& gfx)
	: 
	poo_blue(gfx.MakeSprite(L"../Art/Node_Blue.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_green(gfx.MakeSprite(L"../Art/Node_Green.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_purple(gfx.MakeSprite(L"../Art/Node_Purple.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_red(gfx.MakeSprite(L"../Art/Node_Red.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	rng(std::chrono::system_clock::now().time_since_epoch().count()),
	distribution(1, 4)

{
	this->midPoint = gfx.ScreenWidth / 2.0f;
	this->diameter = 12 * 4;
	poo.push_back(spawnPoo(DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(midPoint, -diameter)));
	
}

PooMachine::~PooMachine()
{
	
}

void PooMachine::update(Graphics& gfx, Keyboard& kbd, float delta)
{
	if (poo.back()->hasLanded)
	{
		poo.push_back(spawnPoo(DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(midPoint, -diameter * 2.0f)));
		poo.push_back(spawnPoo(DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(midPoint, -diameter)));
	}

	//update user input on spawnee
	poo.back()->update(kbd);

	//update graphics and physics
	for (auto p : poo)
	{
		p->update(gfx, delta);
	}
	
}

PooObject* PooMachine::spawnPoo(DirectX::XMFLOAT2 velocity, DirectX::XMFLOAT2 position)
{
	
	PooObject* pooObject = new PooObject(new PooInputComponent(), new PooPhysicsComponent(), new PooGraphicsComponent());
	pooObject->colourType = (PooObject::eColour)distribution(rng);
	reinterpret_cast<PooGraphicsComponent*>(pooObject->graphics)->spritePoo = getSprite(pooObject->colourType);
	pooObject->velocity = velocity;
	pooObject->position = position;

	return pooObject;
}
Sprite* PooMachine::getSprite(PooObject::eColour colour)
{
	switch (colour)
	{
	case PooObject::eColour::BLUE:
		return &poo_blue;
	case PooObject::eColour::GREEN:
		return &poo_green;
	case PooObject::eColour::PURPLE:
		return &poo_purple;
	case PooObject::eColour::RED:
		return &poo_red;
	default:
		return &poo_blue;
		break;
	}
}
