#include "PooComponents.h"
#include "PooObject.h"



using namespace DirectX;

//input
void PooInputComponent::update(GameObject& obj, Keyboard& kbd, float delta)
{
	unsigned char keyCode = kbd.ReadKey().GetCode();
	PooObject& pooObj = dynamic_cast<PooObject&>(obj);
	pooObj.move = Vector2(0.0f, pooObj.speed);

	if (kbd.KeyIsPressed(keyCode))
	{
		switch (keyCode)
		{
		case 'A':
			pooObj.move = Vector2(-pooObj.diameter, 0.0f );
			break;
		case 'D':
			pooObj.move = Vector2(pooObj.diameter, 0.0f );
			break;
		}
	}
	if (kbd.KeyIsPressed('S'))
	{
		pooObj.move = Vector2(0.0f, pooObj.speed * 30.0f);
	}
}
//physics
void PooPhysicsComponent::movement(GameObject& obj, float delta)
{
	PooObject& pooObj = dynamic_cast<PooObject&>(obj);
	if (!pooObj.hasLanded)
	{
		obj.position.y += pooObj.move.y * delta;
		obj.position.x += pooObj.move.x;
	}

	if (pooObj.ptrTandem != NULL)
	{
		pooObj.ptrTandem->position.x = obj.position.x + pooObj.tandemDir.x * pooObj.diameter;
		pooObj.ptrTandem->position.y = obj.position.y + pooObj.tandemDir.y * pooObj.diameter;
	}

}
//graphics
void PooGraphicsComponent::update(GameObject& obj, DirectX::SpriteBatch& batch)
{
	spritePoo->Draw(batch, { obj.position.x, obj.position.y });
}
//bounds collision
void PooPhysicsComponent::collisionBounds(GameObject& obj, float screenWidth, float screenHeight)
{
	if (obj.position.y + obj.diameter > screenHeight)
		obj.physics->collidesType = eCollides::BOUNDS_BOT;
	else if (obj.position.x < 0.0f)
		obj.physics->collidesType = eCollides::BOUNDS_LEFT;
	else if (obj.position.x + obj.diameter > screenWidth)
		obj.physics->collidesType = eCollides::BOUNDS_RIGHT;
}
//resolve bounds collision
void PooPhysicsComponent::resolveBoundsCollision(GameObject& obj, float screenWidth, float screenHeight)
{
	PooObject& pooObj = dynamic_cast<PooObject&>(obj);

	switch (pooObj.physics->collidesType)
	{
	case eCollides::BOUNDS_LEFT:
		pooObj.position.x = 0.0f;
		break;
	case eCollides::BOUNDS_RIGHT:
		pooObj.position.x = screenWidth - obj.diameter;
		break;
	case eCollides::BOUNDS_BOT:
		pooObj.hasLanded = true;
		pooObj.position.y = screenHeight - obj.diameter;

		if (pooObj.ptrTandem != NULL)
		{
			pooObj.ptrTandem->hasLanded = true;
		}
		break;
	}
	pooObj.physics->collidesType = eCollides::DFLT;
	if (pooObj.ptrTandem != NULL)
	{
		pooObj.ptrTandem->position.x = pooObj.position.x + pooObj.tandemDir.x * pooObj.diameter;
		pooObj.ptrTandem->position.y = pooObj.position.y + pooObj.tandemDir.y * pooObj.diameter;
	}
}
//pooyo to pooyo collision
void PooPhysicsComponent::collisionObj(GameObject& obj_Active, GameObject& obj_Inactive)
{
	if (&obj_Active != &obj_Inactive)
	{
		PooObject& dynObjAct = dynamic_cast<PooObject&>(obj_Active);
		PooObject& dynObjInAct = dynamic_cast<PooObject&>(obj_Inactive);

		if (dynObjInAct.hasLanded)
		{
			XMFLOAT2 diff = { dynObjAct.position.x - dynObjInAct.position.x, dynObjAct.position.y - dynObjInAct.position.y };
			float magnitudeSqrd = diff.x * diff.x + diff.y * diff.y;
			
			if (magnitudeSqrd < dynObjAct.diameter * dynObjAct.diameter)
			{
				
				if (dynObjAct.move.x < 0.0f)
				{
					dynObjAct.physics->collidesType = eCollides::LEFT;
				}
				else if (dynObjAct.move.x > 0.0f)
				{
					dynObjAct.physics->collidesType = eCollides::RIGHT;
				}
				else if (dynObjAct.position.y + dynObjAct.diameter > dynObjInAct.position.y)
				{

					dynObjAct.physics->collidesType = eCollides::BOT;
				}
				
			}
		}
	}
}
//resolve obj to obj collision
void PooPhysicsComponent::resolveObjCollision(GameObject& obj_Active, GameObject& obj_Inactive)
{
	PooObject& dynObjAct = dynamic_cast<PooObject&>(obj_Active);
	PooObject& dynObjInAct = dynamic_cast<PooObject&>(obj_Inactive);

	switch (dynObjAct.physics->collidesType)
	{
	case eCollides::LEFT:
		dynObjAct.position.x = dynObjInAct.position.x + dynObjInAct.diameter;
		break;
	case eCollides::RIGHT:
		dynObjAct.position.x = dynObjInAct.position.x - dynObjInAct.diameter;
		break;
	case eCollides::BOT:
		dynObjAct.position.y = dynObjInAct.position.y - dynObjInAct.diameter;
		dynObjAct.hasLanded = true;
		dynObjInAct.hasLanded = true;
		if (dynObjAct.ptrTandem != NULL)
			dynObjAct.ptrTandem->hasLanded = true;
		break;

	}
	dynObjAct.physics->collidesType = eCollides::DFLT;
	if (dynObjAct.ptrTandem != NULL)
	{
		dynObjAct.ptrTandem->position.x = dynObjAct.position.x + dynObjAct.tandemDir.x * dynObjAct.diameter;
		dynObjAct.ptrTandem->position.y = dynObjAct.position.y + dynObjAct.tandemDir.y * dynObjAct.diameter;
	}
}

