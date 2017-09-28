#include "TandemComponents.h"
#include "TandemPooPlCntrlr.h"


using namespace DirectX;

//input
void TandemInptCmpt::update(GameObject& obj, Keyboard& kbd)
{
	TandemPooPlCntrlr& plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);
	unsigned char keyCode = kbd.ReadKey().GetCode();
	//float radians = plCtrlr.rot * plCtrlr.multi;
	plCtrlr.physics->move = Vector2(0.0f, plCtrlr.speed);
	plCtrlr.rotType = TandemPooPlCntrlr::eRotation::ROT_NON;
	
	if (kbd.KeyIsPressed(keyCode))
	{
		switch (keyCode)
		{
		case 'A':
			plCtrlr.physics->move = Vector2(-plCtrlr.diameter, 0.0f);
			break;
		case 'D':
			plCtrlr.physics->move = Vector2(plCtrlr.diameter, 0.0f);
			break;
		case 'Q':
			plCtrlr.multi -= 1.0f;
			plCtrlr.setTandemDir(plCtrlr.rot * plCtrlr.multi);
			plCtrlr.rotType = TandemPooPlCntrlr::eRotation::ROT_LEFT;
			break;
		case 'E':
			plCtrlr.multi += 1.0f;
			plCtrlr.setTandemDir(plCtrlr.rot * plCtrlr.multi);
			plCtrlr.rotType = TandemPooPlCntrlr::eRotation::ROT_RIGHT;
			break;
		}
	}
	if (kbd.KeyIsPressed('S'))
	{
		plCtrlr.physics->move = Vector2(0.0f, plCtrlr.speed * 30.0f);
	}
}
//physics: movement
void TandemPhysicsCmpt::movement(GameObject& obj, float delta)
{
	TandemPooPlCntrlr& plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);
	if (!plCtrlr.mainPoo->hasLanded)
	{
		plCtrlr.mainPoo->position.y += plCtrlr.physics->move.y * delta;
		plCtrlr.mainPoo->position.x += plCtrlr.physics->move.x;
	}
	plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);

}

//physics: bounds collision
void TandemPhysicsCmpt::collisionBounds(GameObject& obj, float screenWidth, float screenHeight)
{ 
	//sets the collision type within the object
	TandemPooPlCntrlr& plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);
	plCtrlr.mainPoo->physics->collisionBounds(*plCtrlr.mainPoo, screenWidth, screenHeight);
	plCtrlr.partnerPoo->physics->collisionBounds(*plCtrlr.partnerPoo, screenWidth, screenHeight);

}
//physics: resolve bounds collision
void TandemPhysicsCmpt::resolveBoundsCollision(GameObject& obj, float screenWidth, float screenHeight)
{
	//cast game object to player controller
	TandemPooPlCntrlr& plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);
	//call resolve colision and update tandem partner
	if (plCtrlr.mainPoo->physics->collidesType != ComponentPhysics::eCollides::DFLT)
	{
		if (plCtrlr.mainPoo->physics->collidesType == ComponentPhysics::eCollides::BOUNDS_BOT)
			plCtrlr.state = TandemPooPlCntrlr::eTandemState::ABANDON;
		plCtrlr.mainPoo->physics->resolveBoundsCollision(*plCtrlr.mainPoo, screenWidth, screenHeight);
		plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);
	}
	if (plCtrlr.partnerPoo->physics->collidesType != ComponentPhysics::eCollides::DFLT)
	{
		if (plCtrlr.rotType == TandemPooPlCntrlr::eRotation::ROT_NON)
		{
			if (plCtrlr.partnerPoo->physics->collidesType == ComponentPhysics::eCollides::BOUNDS_BOT)
				plCtrlr.state = TandemPooPlCntrlr::eTandemState::ABANDON;
			plCtrlr.partnerPoo->physics->resolveBoundsCollision(*plCtrlr.partnerPoo, screenWidth, screenHeight);
			plCtrlr.updateTandem(plCtrlr.partnerPoo, plCtrlr.mainPoo, -1.0f);
		}
		else
		{
			//float radians = 0.0f;
			switch (plCtrlr.rotType)
			{
			case TandemPooPlCntrlr::eRotation::ROT_LEFT:
				plCtrlr.multi += 1.0f;
				//radians = plCtrlr.rot * plCtrlr.multi;
				//plCtrlr.tandemDir = Vector2((int)(cos(radians)), (int)(sin(radians)));
				plCtrlr.setTandemDir(plCtrlr.rot * plCtrlr.multi);
				plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);
				break;
			case TandemPooPlCntrlr::eRotation::ROT_RIGHT:
				plCtrlr.multi -= 1.0f;
				//radians = plCtrlr.rot * plCtrlr.multi;
				//plCtrlr.tandemDir = Vector2((int)(cos(radians)), (int)(sin(radians)));
				plCtrlr.setTandemDir(plCtrlr.rot * plCtrlr.multi);
				plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);
				break;

			}
			plCtrlr.rotType = TandemPooPlCntrlr::eRotation::ROT_NON;
		}
	}

}
//physics: pooyo to pooyo collision
void TandemPhysicsCmpt::collisionObj(GameObject& obj, GameObject& obj_Inactive)
{
	TandemPooPlCntrlr& plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);
	plCtrlr.mainPoo->physics->collisionObj(*plCtrlr.mainPoo, obj_Inactive);
	if (plCtrlr.mainPoo->hasCollided)
		plCtrlr.setCollisionType(plCtrlr.mainPoo);
	
	plCtrlr.partnerPoo->physics->collisionObj(*plCtrlr.partnerPoo, obj_Inactive);
	if (plCtrlr.partnerPoo->hasCollided)
		plCtrlr.setCollisionType(plCtrlr.partnerPoo);

}
//physics: resolve obj to obj collision
void TandemPhysicsCmpt::resolveObjCollision(GameObject& obj, GameObject& obj_Inactive)
{
	TandemPooPlCntrlr& plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);
	
	if (plCtrlr.mainPoo->hasCollided)
	{

		if (plCtrlr.mainPoo->physics->collidesType == ComponentPhysics::eCollides::BOT)
		{
			plCtrlr.mainPoo->physics->resolveObjCollision(*plCtrlr.mainPoo, obj_Inactive);
			plCtrlr.state = TandemPooPlCntrlr::eTandemState::ABANDON;
			plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);
		}
		else
		{
			plCtrlr.mainPoo->physics->resolveObjCollision(*plCtrlr.mainPoo, obj_Inactive);
			plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);
			plCtrlr.mainPoo->hasCollided = false;
		}
	}
	else if (plCtrlr.partnerPoo->hasCollided)
	{
		if (plCtrlr.rotType == TandemPooPlCntrlr::eRotation::ROT_NON)
		{
			if (plCtrlr.partnerPoo->physics->collidesType == ComponentPhysics::eCollides::BOT)
			{
				plCtrlr.partnerPoo->physics->resolveObjCollision(*plCtrlr.partnerPoo, obj_Inactive);
				plCtrlr.state = TandemPooPlCntrlr::eTandemState::ABANDON;
				plCtrlr.updateTandem(plCtrlr.partnerPoo, plCtrlr.mainPoo, -1.0f);
			}
			else
			{
				plCtrlr.partnerPoo->physics->resolveObjCollision(*plCtrlr.partnerPoo, obj_Inactive);
				plCtrlr.updateTandem(plCtrlr.partnerPoo, plCtrlr.mainPoo, -1.0f);
			}
		}
		else
		{
			//float radians = 0.0f;
			switch (plCtrlr.rotType)
			{
			case TandemPooPlCntrlr::eRotation::ROT_LEFT:
				plCtrlr.multi += 1.0f;
				//radians = plCtrlr.rot * plCtrlr.multi;
				//plCtrlr.tandemDir = Vector2((int)(cos(radians)), (int)(sin(radians)));
				plCtrlr.setTandemDir(plCtrlr.rot * plCtrlr.multi);
				plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);
				break;
			case TandemPooPlCntrlr::eRotation::ROT_RIGHT:
				plCtrlr.multi -= 1.0f;
				//radians = plCtrlr.rot * plCtrlr.multi;
				//plCtrlr.tandemDir = Vector2((int)(cos(radians)), (int)(sin(radians)));
				plCtrlr.setTandemDir(plCtrlr.rot * plCtrlr.multi);
				plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);
				break;

			}
			plCtrlr.rotType = TandemPooPlCntrlr::eRotation::ROT_NON;
		}
	}
	
	plCtrlr.partnerPoo->hasCollided = false;

}
void TandemGraphicsCmpt::draw(GameObject& obj, DirectX::SpriteBatch& batch)
{
	TandemPooPlCntrlr& plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);

	plCtrlr.mainPoo->draw(batch);
	plCtrlr.partnerPoo->draw(batch);
}