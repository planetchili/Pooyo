#include "TandemComponents.h"
#include "TandemPooPlCntrlr.h"


using namespace DirectX;


void TandemInptCmpt::update(TandemPooPlCntrlr& plCtrlr, Keyboard& kbd, float delta)
{
	unsigned char keyCode = kbd.ReadKey().GetCode();

	plCtrlr.move = Vector2(0.0f, plCtrlr.speed);

	if (kbd.KeyIsPressed(keyCode))
	{
		switch (keyCode)
		{
		case 'A':
			plCtrlr.move = Vector2(-plCtrlr.diameter, 0.0f);
			break;
		case 'D':
			plCtrlr.move = Vector2(plCtrlr.diameter, 0.0f);
			break;
		}
	}
	if (kbd.KeyIsPressed('S'))
	{
		plCtrlr.move = Vector2(0.0f, plCtrlr.speed * 30.0f);
	}
}
//physics
void TandemPhysicsCmpt::movement(TandemPooPlCntrlr& plCtrlr, float delta)
{

	if (!plCtrlr.mainPoo->hasLanded)
	{
		plCtrlr.mainPoo->position.y += plCtrlr.move.y * delta;
		plCtrlr.mainPoo->position.x += plCtrlr.move.x;
	}

	if (plCtrlr.partnerPoo != NULL)
	{
		plCtrlr.partnerPoo->position = plCtrlr.mainPoo->position + plCtrlr.tandemDir * plCtrlr.diameter;
	}

}

//bounds collision
void TandemPhysicsCmpt::collisionBounds(TandemPooPlCntrlr& plCtrlr, float screenWidth, float screenHeight)
{
	if (plCtrlr.mainPoo->position.y + plCtrlr.diameter > screenHeight)
		plCtrlr.mainPoo->physics->collidesType = eCollides::BOUNDS_BOT;
	else if (plCtrlr.mainPoo->position.x < 0.0f)
		plCtrlr.mainPoo->physics->collidesType = eCollides::BOUNDS_LEFT;
	else if (plCtrlr.mainPoo->position.x + plCtrlr.diameter > screenWidth)
		plCtrlr.mainPoo->physics->collidesType = eCollides::BOUNDS_RIGHT;
}
//resolve bounds collision
void TandemPhysicsCmpt::resolveBoundsCollision(TandemPooPlCntrlr& plCtrlr, float screenWidth, float screenHeight)
{

	switch (plCtrlr.mainPoo->physics->collidesType)
	{
	case eCollides::BOUNDS_LEFT:
		plCtrlr.mainPoo->position.x = 0.0f;
		break;
	case eCollides::BOUNDS_RIGHT:
		plCtrlr.mainPoo->position.x = screenWidth - plCtrlr.diameter;
		break;
	case eCollides::BOUNDS_BOT:
		plCtrlr.mainPoo->hasLanded = true;
		plCtrlr.mainPoo->position.y = screenHeight - plCtrlr.diameter;

		if (plCtrlr.partnerPoo != NULL)
		{
			plCtrlr.partnerPoo->hasLanded = true;
		}
		break;
	}
	plCtrlr.mainPoo->physics->collidesType = eCollides::DFLT;

	if (plCtrlr.partnerPoo != NULL)
	{
		plCtrlr.partnerPoo->position = plCtrlr.mainPoo->position + plCtrlr.tandemDir * plCtrlr.diameter;
	}
}
//pooyo to pooyo collision
void TandemPhysicsCmpt::collisionObj(TandemPooPlCntrlr& plCtrlr, GameObject& obj_Inactive)
{
	if (plCtrlr.mainPoo != &obj_Inactive)
	{
		PooObject& dynObjInAct = dynamic_cast<PooObject&>(obj_Inactive);

		if (dynObjInAct.hasLanded)
		{
			Vector2 diff =  plCtrlr.mainPoo->position - dynObjInAct.position; 

			if (diff.LengthSquared() < pow(plCtrlr.diameter, 2.0f));
			{

				if (plCtrlr.move.x < 0.0f)
				{
					plCtrlr.mainPoo->physics->collidesType = eCollides::LEFT;
				}
				else if (plCtrlr.move.x > 0.0f)
				{
					plCtrlr.mainPoo->physics->collidesType = eCollides::RIGHT;
				}
				else if (plCtrlr.mainPoo->position.y + plCtrlr.diameter > dynObjInAct.position.y)
				{

					plCtrlr.mainPoo->physics->collidesType = eCollides::BOT;
				}
			}
		}
	}
}
//resolve obj to obj collision
void TandemPhysicsCmpt::resolveObjCollision(TandemPooPlCntrlr& plCtrlr, GameObject& obj_Inactive)
{
	PooObject& dynObjInAct = dynamic_cast<PooObject&>(obj_Inactive);

	switch (plCtrlr.mainPoo->physics->collidesType)
	{
	case eCollides::LEFT:
		plCtrlr.mainPoo->position.x = dynObjInAct.position.x + dynObjInAct.diameter;
		break;
	case eCollides::RIGHT:
		plCtrlr.mainPoo->position.x = dynObjInAct.position.x - dynObjInAct.diameter;
		break;
	case eCollides::BOT:
		plCtrlr.mainPoo->position.y = dynObjInAct.position.y - dynObjInAct.diameter;
		plCtrlr.mainPoo->hasLanded = true;
		dynObjInAct.hasLanded = true;
		if (plCtrlr.partnerPoo != NULL)
			plCtrlr.partnerPoo->hasLanded = true;
		break;

	}
	plCtrlr.mainPoo->physics->collidesType = eCollides::DFLT;
	if (plCtrlr.partnerPoo != NULL)
	{
		plCtrlr.partnerPoo->position = plCtrlr.mainPoo->position + plCtrlr.tandemDir * plCtrlr.diameter;
	}
}