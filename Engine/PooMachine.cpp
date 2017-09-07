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
	this->tandemPooPlcntrlr = new TandemPooPlCntrlr(new TandemInptCmpt(), new TandemPhysicsCmpt(), new TandemGraphicsCmpt());
	spawnTandemPoo();
}

PooMachine::~PooMachine()
{
	
}

void PooMachine::update(Graphics& gfx, Keyboard& kbd, float delta)
{
	//spawn new tandem poo once previous tandem poo has landed
	if (tandemPooPlcntrlr->mainPoo->hasLanded)
	{
		spawnTandemPoo();
	}

	//update user input on spawnee
	//poo.back()->update(kbd, delta);
	this->tandemPooPlcntrlr->update(kbd);

	//update physics
	//poo.back()->update((float)gfx.ScreenWidth, (float)gfx.ScreenHeight, delta);
	this->tandemPooPlcntrlr->update((float)gfx.ScreenWidth, (float)gfx.ScreenHeight, delta);

	//update collision
	for (auto p : poo)
	{
		p->update(*this->tandemPooPlcntrlr->mainPoo);
		//this->tandemPooPlcntrlr->update(*p);
		
	}
	
	//update graphics
	auto batch = gfx.MakeSpriteBatch();
	batch.Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates().NonPremultiplied(), gfx.GetStates().PointClamp());

	for (auto p : poo)
	{
		p->draw(batch);
	}
	this->tandemPooPlcntrlr->draw(batch);


	batch.End();
}

void PooMachine::createTandemPooObj(float x, float y)
{
	if (tandemPooPlcntrlr->mainPoo != NULL)
	{
		poo.push_back(tandemPooPlcntrlr->mainPoo);
		poo.push_back(tandemPooPlcntrlr->partnerPoo);
	}
	
	tandemPooPlcntrlr->mainPoo = new PooObject( NULL, new PooPhysicsComponent(), new PooGraphicsComponent());
	tandemPooPlcntrlr->mainPoo->colourType = (PooObject::eColour)distribution(rng);
	reinterpret_cast<PooGraphicsComponent*>(tandemPooPlcntrlr->mainPoo->graphics)->spritePoo = getSprite(tandemPooPlcntrlr->mainPoo->colourType);
	
	tandemPooPlcntrlr->mainPoo->position = Vector2( x, y );

	tandemPooPlcntrlr->partnerPoo = new PooObject(NULL, NULL, new PooGraphicsComponent());
	tandemPooPlcntrlr->partnerPoo->colourType = (PooObject::eColour)distribution(rng);
	reinterpret_cast<PooGraphicsComponent*>(tandemPooPlcntrlr->partnerPoo->graphics)->spritePoo = getSprite(tandemPooPlcntrlr->partnerPoo->colourType);


}
void PooMachine::spawnTandemPoo()
{
	createTandemPooObj(diameter * 2.0f, -diameter * 2.0f);
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
