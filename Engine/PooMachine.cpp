#include "PooMachine.h"

PooMachine::PooMachine(Graphics& gfx)
	: 
	poo_blue(gfx.MakeSprite(L"../Art/Node_Blue.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_green(gfx.MakeSprite(L"../Art/Node_Green.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_purple(gfx.MakeSprite(L"../Art/Node_Purple.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	poo_red(gfx.MakeSprite(L"../Art/Node_Red.png", { 0,0,12,12 }, 4.0f, { 0.0f,0.0f })),
	rng((unsigned int)std::chrono::system_clock::now().time_since_epoch().count()),
	distribution(0, 3),
	pooyo({6}),
	state(SPAWN)
{
	this->midPoint = gfx.ScreenWidth / 2.0f;
	this->diameter = 12 * 4;
	this->tandemPooPlcntrlr = new TandemPooPlCntrlr(new TandemInptCmpt(), new TandemPhysicsCmpt(), new TandemGraphicsCmpt());
	this->tandemPooPlcntrlr->state = TandemPooPlCntrlr::eTandemState::DFLT;
	
}

PooMachine::~PooMachine()
{
	
}

void PooMachine::update(Graphics& gfx, Keyboard& kbd, float delta)
{
	

	//switch (tandemPooPlcntrlr->state)
	//{
	//case TandemPooPlCntrlr::eTandemState::SPAWN:
	//	
	//	break;
	//case TandemPooPlCntrlr::eTandemState::ACTIVE://PLAY
	//	
	//for (auto py : pooyo)
	//{
	//	for (auto p : py)
	//	{
	//		if (tandemPooPlcntrlr->state != TandemPooPlCntrlr::eTandemState::ABANDON)//change to while loop
	//			this->tandemPooPlcntrlr->update(*p);
	//		//else change state to LIMBO
	//	}
	//
	//}
	//	break;
	////case TandemPooPlCntrlr::eTandemState::DISMOUNT://LIMBO
	//	
	//	//break;
	//case TandemPooPlCntrlr::eTandemState::DISJOINT://FREEFALL
	//	
	//
	//		//check for adjacent match-ups
			//connectPooyo(tandemPooPlcntrlr->mainPoo);
			//connectPooyo(tandemPooPlcntrlr->partnerPoo);
			//check connections > 4
			//if (tandemPooPlcntrlr->partnerPoo->sequenceNum > tandemPooPlcntrlr->mainPoo->sequenceNum)
			//{
			//	if (tandemPooPlcntrlr->partnerPoo->sequenceNum >= 4)
			//		removeGroup(tandemPooPlcntrlr->partnerPoo);
			//}
			//else if (tandemPooPlcntrlr->mainPoo->sequenceNum >= 4)
			//{
			//	removeGroup(tandemPooPlcntrlr->mainPoo);
			//}
			//else if (tandemPooPlcntrlr->partnerPoo->sequenceNum >= 4)
			//{
			//	removeGroup(tandemPooPlcntrlr->partnerPoo);
			//}
			//tandemPooPlcntrlr->mainPoo = NULL;
			//tandemPooPlcntrlr->partnerPoo = NULL;
			////spawn tandem
			//tandemPooPlcntrlr->state = TandemPooPlCntrlr::eTandemState::SPAWN;
	//	
	//	break;
	//case TandemPooPlCntrlr::eTandemState::DFLT:
	//	break;
	//	
	//}
	switch (state)
	{
	case SPAWN:
		spawnTandemPoo();
		this->state = eMachineState::PLAY;
		break;
	case PLAY:
		//update user input on spawnee
		this->tandemPooPlcntrlr->update(kbd);
		//update physics
		this->tandemPooPlcntrlr->update((float)gfx.ScreenWidth, (float)gfx.ScreenHeight, delta);
		if (tandemPooPlcntrlr->state == TandemPooPlCntrlr::eTandemState::ABANDON)
			this->state = eMachineState::PLACE;
		else
			//update pooyo collision
			this->update_collision_tandem();
		break;
	case PLACE:
		if (tandemPooPlcntrlr->mainPoo->position.y > tandemPooPlcntrlr->partnerPoo->position.y)
		{
			placePooyo(tandemPooPlcntrlr->mainPoo);
			placePooyo(tandemPooPlcntrlr->partnerPoo);
		}
		else
		{
			placePooyo(tandemPooPlcntrlr->partnerPoo);
			placePooyo(tandemPooPlcntrlr->mainPoo);

		}
		this->state = eMachineState::FREEFALL;
		break;
	case FREEFALL:
		//update collision for freefalling
		update_collision_pooyo(gfx, delta);

		if (tandemPooPlcntrlr->mainPoo->hasLanded && tandemPooPlcntrlr->partnerPoo->hasLanded)
		{
			//add to queue for connection checking
			checkPoo.push_back(tandemPooPlcntrlr->mainPoo);
			checkPoo.push_back(tandemPooPlcntrlr->partnerPoo);
			tandemPooPlcntrlr->mainPoo = NULL;		//this is done in TandemController.reset() consider removing
			tandemPooPlcntrlr->partnerPoo = NULL;	//this is done in TandemController.reset() consider removing
			this->state = eMachineState::CONNECT;
		}
		break;
	case CONNECT:
		//check for adjacent match-ups
		checkAdjMatchUps();

		this->state = eMachineState::REMOVE;
		break;
	case REMOVE:
		if (checkPoo.empty())
		{
			this->state = eMachineState::SPAWN;
		}
		else
		{
			
			int numPoo = checkPoo.size();
			while (numPoo != 0)
			{

				if (checkPoo.front()->sequenceNum > 3)
				{
					removeGroup(checkPoo.front());
					cleanUpPooyo();
				}
				checkPoo.pop_front();
				numPoo--;
			}
			checkPoo.sort();
			checkPoo.unique();
			this->state = eMachineState::CHAINS;
		}
		break;
	case CHAINS:

		update_collision_pooyo(gfx, delta);
		if (this->columnsHasLanded())
			this->state = eMachineState::CONNECT;
		break;
	case DFLT:
		break;
	}
	//update graphics
	update_GFX(gfx);
}
bool PooMachine::columnsHasLanded()
{
	bool allLanded = true;
	auto it = columns.begin();
	while (allLanded && it != columns.end())
	{
		if (!pooyo[*it].empty())
		{
			if (!pooyo[*it].back()->hasLanded)
				allLanded = false;
		}
		it++;
	}
	return allLanded;
}
void PooMachine::checkAdjMatchUps()
{
	int it = 0;
	while (it < checkPoo.size())
	{
		
		connectPooyo(checkPoo.front());
		checkPoo.push_back(checkPoo.front());
		checkPoo.pop_front();
		it++;

	}
}
void PooMachine::update_GFX(Graphics& gfx)
{
	auto batch = gfx.MakeSpriteBatch();
	batch.Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates().NonPremultiplied(), gfx.GetStates().PointClamp());
	for (auto py : pooyo)
	{
		for (auto p : py)
			p->draw(batch);
	}
	this->tandemPooPlcntrlr->draw(batch);
	batch.End();
}
void PooMachine::update_collision_tandem()
{
	auto it_pooyo = pooyo.begin();
	while (it_pooyo != pooyo.end())
	{
		auto it_poo = (*it_pooyo).begin();
		while (it_poo != (*it_pooyo).end())
		{
			if (tandemPooPlcntrlr->state != TandemPooPlCntrlr::eTandemState::ABANDON)
			{
				this->tandemPooPlcntrlr->update(*(*it_poo));
				it_poo++;
			}
			else
			{
				it_poo = (*it_pooyo).end();
			}
		}
		if (tandemPooPlcntrlr->state == TandemPooPlCntrlr::eTandemState::ABANDON)
		{
			it_pooyo = pooyo.end();
			this->state = eMachineState::PLACE;
		}
		else
			it_pooyo++;
	}
		
}
void PooMachine::update_collision_pooyo(Graphics& gfx, float delta)
{
	for (auto py : pooyo)
	{
		for (auto p : py)
		{
			p->update((float)gfx.ScreenWidth, (float)gfx.ScreenHeight, delta);
			for (auto q : py)
			{
				p->update(*q);
			}
		}
	}
}
void PooMachine::createTandemPooObj(float x, float y)
{
	
	
	tandemPooPlcntrlr->mainPoo = new PooObject( NULL, new PooPhysicsComponent(), new PooGraphicsComponent());
	tandemPooPlcntrlr->mainPoo->colourType = (PooObject::eColour)distribution(rng);
	reinterpret_cast<PooGraphicsComponent*>(tandemPooPlcntrlr->mainPoo->graphics)->spritePoo = getSprite(tandemPooPlcntrlr->mainPoo->colourType);
	
	tandemPooPlcntrlr->mainPoo->position = Vector2( x, y );

	tandemPooPlcntrlr->partnerPoo = new PooObject(NULL, new PooPhysicsComponent(), new PooGraphicsComponent());
	tandemPooPlcntrlr->partnerPoo->colourType = (PooObject::eColour)distribution(rng);
	reinterpret_cast<PooGraphicsComponent*>(tandemPooPlcntrlr->partnerPoo->graphics)->spritePoo = getSprite(tandemPooPlcntrlr->partnerPoo->colourType);

	tandemPooPlcntrlr->updateTandem(tandemPooPlcntrlr->mainPoo, tandemPooPlcntrlr->partnerPoo);
}
void PooMachine::spawnTandemPoo()
{
	tandemPooPlcntrlr->reset();
	
	createTandemPooObj(diameter * 2.0f, -diameter * 1.0f);
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
void PooMachine::placePooyo(PooObject* poo)
{
	pooyo[(int)(poo->position.x / poo->diameter)].push_back(poo);
}
void PooMachine::connectPooyo(PooObject* poo)
{
	int MAX_HEIGHT = 12;
	int x = (int)poo->position.x / (int)poo->diameter;
	int y = MAX_HEIGHT - ((int)poo->position.y / (int)poo->diameter) - 1;
	//left
	if (checkBounds(x - 1, y))
		poo->connectPoo(pooyo[x - 1][y]);
	//bot
	if (checkBounds(x, y - 1))
		poo->connectPoo(pooyo[x][y - 1]);
	//right
	if (checkBounds(x + 1, y))
		poo->connectPoo(pooyo[x + 1][y]);
}
bool PooMachine::checkBounds(const int x, const int y)
{
	int MAX_WIDTH = 6;
	if (x >= 0 && x < MAX_WIDTH)
			if (pooyo[x].size() > y)
				return true;
	return false;
}
void PooMachine::removeGroup(PooObject* poo)
{
	PooObject* curPoo = poo->ptrHeadPoo;
	PooObject* oldPoo = NULL;
	int MAX_HEIGHT = 12;
	int x = 0;
	int y = 0;
	columns.clear();
	do
	{
		x = (int)curPoo->position.x / (int)curPoo->diameter;
		y = MAX_HEIGHT - ((int)curPoo->position.y / (int)curPoo->diameter) - 1;
		if (checkBounds(x, y))
		{
			oldPoo = curPoo;
			curPoo = curPoo->ptrNextPoo;
			oldPoo->ptrHeadPoo = NULL;//make PooObject.dispose() instead of preformaing all removal requirements here
			oldPoo->ptrNextPoo = NULL;
			delete oldPoo;
			oldPoo = NULL;
			pooyo[x][y] = NULL;
			columns.push_back(x);
		}
	} while (curPoo != NULL);

}
void PooMachine::resetGroup(PooObject* poo)
{
	PooObject* curPoo = poo->ptrHeadPoo;
	PooObject* oldPoo = NULL;
	while (curPoo != NULL)
	{
		oldPoo = curPoo;
		curPoo = oldPoo->ptrNextPoo;

		oldPoo->ptrHeadPoo = NULL;
		oldPoo->ptrNextPoo = NULL;
		oldPoo->sequenceNum = 1;
		checkPoo.push_back(oldPoo);
	}
}
void PooMachine::cleanUpPooyo()
{
	bool hasRemoved = false;
	columns.sort();
	columns.unique();
	for (auto c : columns)
	{
		hasRemoved = false;//reset to false for each column
		auto it = pooyo[c].begin();
		while (it != pooyo[c].end())
		{
			if ((*it) == NULL)
			{
				it = pooyo[c].erase(it);
				hasRemoved = true;
			}
			else
			{
				if (hasRemoved)						//? has removed doesnt get reset within loop ? now gets rest for each column
				{
					(*it)->hasLanded = false;		//new code
					resetGroup((*it));
					checkPoo.push_back((*it));
				}
				it++;
			}
			
		}
	}
	
}