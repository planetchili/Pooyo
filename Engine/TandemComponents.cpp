#include "TandemComponents.h"
#include "TandemPooPlCntrlr.h"


using namespace DirectX;

//input
void TandemInptCmpt::update(GameObject& obj, Keyboard& kbd)
{
	TandemPooPlCntrlr& plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);
	unsigned char keyCode = kbd.ReadKey().GetCode();
	float radians = plCtrlr.rot * plCtrlr.multi;
	plCtrlr.physics->move = Vector2(0.0f, plCtrlr.speed);

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
			radians = plCtrlr.rot * plCtrlr.multi;
			plCtrlr.tandemDir = Vector2((int)(cos(radians)), (int)(sin(radians)));
			break;
		case 'E':
			plCtrlr.multi += 1.0f;
			radians = plCtrlr.rot * plCtrlr.multi;
			plCtrlr.tandemDir = Vector2((int)(cos(radians)), (int)(sin(radians)));
			
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

	//if (plCtrlr.partnerPoo != NULL)
	//{
	//	plCtrlr.partnerPoo->position = plCtrlr.mainPoo->position + plCtrlr.tandemDir * plCtrlr.diameter;
	//}

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
			plCtrlr.state = TandemPooPlCntrlr::eTandemState::DISMOUNT;
		plCtrlr.mainPoo->physics->resolveBoundsCollision(*plCtrlr.mainPoo, screenWidth, screenHeight);
		plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);
		//plCtrlr.partnerPoo->position = plCtrlr.mainPoo->position + plCtrlr.tandemDir * plCtrlr.diameter;
	}
	if (plCtrlr.partnerPoo->physics->collidesType != ComponentPhysics::eCollides::DFLT)
	{
		if (plCtrlr.partnerPoo->physics->collidesType == ComponentPhysics::eCollides::BOUNDS_BOT)
			plCtrlr.state = TandemPooPlCntrlr::eTandemState::DISMOUNT;
		plCtrlr.partnerPoo->physics->resolveBoundsCollision(*plCtrlr.partnerPoo, screenWidth, screenHeight);
		plCtrlr.updateTandem(plCtrlr.partnerPoo, plCtrlr.mainPoo, -1.0f);
		//plCtrlr.mainPoo->position = plCtrlr.partnerPoo->position - plCtrlr.tandemDir * plCtrlr.diameter;
	}
	//if resting at bottom of container then both has landed
	//    - some adjustements will need to be made for when tandem need to seperate into single pooyo
	//    - need to consider a turn based game also 
	//    - dont spawn new tandem pooyo until both pooyo have landed
	if (plCtrlr.mainPoo->hasLanded)
	{

		//plCtrlr.partnerPoo->hasLanded = true;
		
	}
	else if (plCtrlr.partnerPoo->hasLanded)
	{

		//plCtrlr.mainPoo->hasLanded = true;
		
	}

	//below is for reference only and will be deleted as has been moved to pooComonent collision
	//switch (plCtrlr.mainPoo->physics->collidesType)
	//{
	//case eCollides::BOUNDS_LEFT:
	//	plCtrlr.mainPoo->position.x = 0.0f;
	//	break;
	//case eCollides::BOUNDS_RIGHT:
	//	plCtrlr.mainPoo->position.x = screenWidth - plCtrlr.diameter;
	//	break;
	//case eCollides::BOUNDS_BOT:
	//	plCtrlr.mainPoo->hasLanded = true;
	//	plCtrlr.mainPoo->position.y = screenHeight - plCtrlr.diameter;
	//
	//	if (plCtrlr.partnerPoo != NULL)
	//	{
	//		plCtrlr.partnerPoo->hasLanded = true;
	//	}
	//	break;
	//}
	//plCtrlr.mainPoo->physics->collidesType = eCollides::DFLT;

	
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
	//TandemPooPlCntrlr plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);
	//
	//if (plCtrlr.mainPoo != &obj_Inactive)
	//{
	//	PooObject& dynObjInAct = dynamic_cast<PooObject&>(obj_Inactive);
	//
	//	if (dynObjInAct.hasLanded)
	//	{
	//		Vector2 diff =  plCtrlr.mainPoo->position - dynObjInAct.position; 
	//
	//		if (diff.LengthSquared() < pow(plCtrlr.diameter, 2.0f))
	//		{
	//
	//			if (plCtrlr.physics->move.x < 0.0f)
	//			{
	//				plCtrlr.mainPoo->physics->collidesType = eCollides::LEFT;
	//			}
	//			else if (plCtrlr.physics->move.x > 0.0f)
	//			{
	//				plCtrlr.mainPoo->physics->collidesType = eCollides::RIGHT;
	//			}
	//			else if (plCtrlr.mainPoo->position.y + plCtrlr.diameter > dynObjInAct.position.y)
	//			{
	//
	//				plCtrlr.mainPoo->physics->collidesType = eCollides::BOT;
	//			}
	//		}
	//	}
	//}
}
//physics: resolve obj to obj collision
void TandemPhysicsCmpt::resolveObjCollision(GameObject& obj, GameObject& obj_Inactive)
{
	TandemPooPlCntrlr& plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);
	
	
	if (plCtrlr.mainPoo->physics->collidesType == ComponentPhysics::eCollides::BOT)
	{
		plCtrlr.mainPoo->physics->resolveObjCollision(*plCtrlr.mainPoo, obj_Inactive);
		plCtrlr.state = TandemPooPlCntrlr::eTandemState::DISMOUNT;
		plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);
	}
	else if (plCtrlr.partnerPoo->physics->collidesType == ComponentPhysics::eCollides::BOT)
	{
		plCtrlr.partnerPoo->physics->resolveObjCollision(*plCtrlr.partnerPoo, obj_Inactive);
		plCtrlr.state = TandemPooPlCntrlr::eTandemState::DISMOUNT;
		plCtrlr.updateTandem(plCtrlr.partnerPoo, plCtrlr.mainPoo, -1.0f);
	}
	else
	{
		if (plCtrlr.mainPoo->hasCollided)
		{
			plCtrlr.mainPoo->physics->resolveObjCollision(*plCtrlr.mainPoo, obj_Inactive);
			plCtrlr.updateTandem(plCtrlr.mainPoo, plCtrlr.partnerPoo);
			plCtrlr.mainPoo->hasCollided = false;
		}
		else if (plCtrlr.partnerPoo->hasCollided)
		{
			plCtrlr.partnerPoo->physics->resolveObjCollision(*plCtrlr.partnerPoo, obj_Inactive);
			plCtrlr.updateTandem(plCtrlr.partnerPoo, plCtrlr.mainPoo, -1.0f);
			plCtrlr.partnerPoo->hasCollided = false;
		}
	}
	
	
	//PooObject& dynObjInAct = dynamic_cast<PooObject&>(obj_Inactive);

	//switch (plCtrlr.mainPoo->physics->collidesType)
	//{
	//case eCollides::LEFT:
	//	plCtrlr.mainPoo->position.x = dynObjInAct.position.x + dynObjInAct.diameter;
	//	break;
	//case eCollides::RIGHT:
	//	plCtrlr.mainPoo->position.x = dynObjInAct.position.x - dynObjInAct.diameter;
	//	break;
	//case eCollides::BOT:
	//	plCtrlr.mainPoo->position.y = dynObjInAct.position.y - dynObjInAct.diameter;
	//	plCtrlr.mainPoo->hasLanded = true;
	//	dynObjInAct.hasLanded = true;
	//	if (plCtrlr.partnerPoo != NULL)
	//		plCtrlr.partnerPoo->hasLanded = true;
	//	break;
	//
	//}
	//plCtrlr.mainPoo->physics->collidesType = eCollides::DFLT;
	//if (plCtrlr.partnerPoo != NULL)
	//{
	//	plCtrlr.partnerPoo->position = plCtrlr.mainPoo->position + plCtrlr.tandemDir * plCtrlr.diameter;
	//}
}
void TandemGraphicsCmpt::draw(GameObject& obj, DirectX::SpriteBatch& batch)
{
	TandemPooPlCntrlr& plCtrlr = dynamic_cast<TandemPooPlCntrlr&>(obj);

	plCtrlr.mainPoo->draw(batch);
	plCtrlr.partnerPoo->draw(batch);
}