#include "PooMachine.h"

PooMachine::PooMachine(Graphics& gfx)
	: 
	poo_blue(gfx.MakeSprite(L"../Art/Node_Blue.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_green(gfx.MakeSprite(L"../Art/Node_Green.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_purple(gfx.MakeSprite(L"../Art/Node_Purple.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_red(gfx.MakeSprite(L"../Art/Node_Red.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	rng((unsigned int)std::chrono::system_clock::now().time_since_epoch().count()),
	distribution(0, 3)

{
	this->midPoint = gfx.ScreenWidth / 2.0f;
	this->diameter = 12 * 4;
	spawnTandemPoo();
}

PooMachine::~PooMachine()
{
	
}

void PooMachine::update(Graphics& gfx, Keyboard& kbd, float delta)
{
	//spawn new tandem poo once previous tandem poo has landed
	if (poo.back()->hasLanded)
	{
		spawnTandemPoo();
	}

	//update user input on spawnee
	poo.back()->update(kbd, delta);

	//update physics
	poo.back()->update((float)gfx.ScreenWidth, (float)gfx.ScreenHeight, delta);

	//update collision
	for (auto p : poo)
	{
		p->update(*poo.back());
	}
	
	//update graphics
	auto batch = gfx.MakeSpriteBatch();
	batch.Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates().NonPremultiplied(), gfx.GetStates().PointClamp());

	for (auto p : poo)
	{
		p->update(batch);
	}

	batch.End();
}

PooObject* PooMachine::createPooObj(float x, float y)
{
	
	PooObject* pooObject = new PooObject(new PooInputComponent(), new PooPhysicsComponent(), new PooGraphicsComponent());
	pooObject->colourType = (PooObject::eColour)distribution(rng);
	reinterpret_cast<PooGraphicsComponent*>(pooObject->graphics)->spritePoo = getSprite(pooObject->colourType);
	
	pooObject->position = Vector2( x, y );

	return pooObject;
}
void PooMachine::spawnTandemPoo()
{
	poo.push_back(createPooObj(diameter * 2.0f, -diameter * 2.0f));
	poo.push_back(createPooObj(diameter * 2.0f, -diameter ));
	poo.back()->ptrTandem = poo[poo.size() - 2];
	poo.back()->tandemDir.y = -1.0f;
	poo.back()->tandemDir.x = +0.0f;
	poo[poo.size() - 2]->ptrTandem = poo.back();
	poo[poo.size() - 2]->tandemDir.y = 1.0f;
	poo[poo.size() - 2]->tandemDir.x = 0.0f;
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
