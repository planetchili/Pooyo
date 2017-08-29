#include "PooComponents.h"
#include "PooObject.h"



using namespace DirectX;


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
			//pooObj.position.x += -1.0f * pooObj.diameter;
			break;
		case 'D':
			pooObj.move = Vector2(pooObj.diameter, 0.0f );
			//pooObj.position.x += 1.0f * pooObj.diameter;
			break;
		}
	}
	if (kbd.KeyIsPressed('S'))
	{
		pooObj.move = Vector2(0.0f, pooObj.speed * 30.0f);
		//pooObj.position.y += delta * pooObj.speed * 20.0f;
	}
}

void PooPhysicsComponent::update(GameObject& obj, float delta, Graphics& gfx)
{
	PooObject &pooObj = dynamic_cast<PooObject&>(obj);
	if (!pooObj.hasLanded)
	{
		obj.position.y += pooObj.move.y * delta;
		obj.position.x += pooObj.move.x;
	}

	if (obj.position.y + obj.diameter > gfx.ScreenHeight)
	{
		pooObj.hasLanded = true;
		obj.position.y = gfx.ScreenHeight - obj.diameter;

		if (pooObj.ptrTandem != NULL)
		{
			pooObj.ptrTandem->hasLanded = true;
		}
	}
	if (pooObj.ptrTandem != NULL)
	{
		pooObj.ptrTandem->position.x = obj.position.x + pooObj.tandemDir.x * pooObj.diameter;
		pooObj.ptrTandem->position.y = obj.position.y + pooObj.tandemDir.y * pooObj.diameter;
	}

}

void PooGraphicsComponent::update(GameObject& obj, DirectX::SpriteBatch& batch)
{
	spritePoo->Draw(batch, { obj.position.x, obj.position.y });
}

void PooPhysicsComponent::collision(GameObject& obj_Active, GameObject& obj_Inactive)
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
					dynObjAct.position.x = dynObjInAct.position.x + dynObjInAct.diameter;
				}
				else if (dynObjAct.move.x > 0.0f)
				{
					dynObjAct.position.x = dynObjInAct.position.x - dynObjInAct.diameter;
				}
				else if (dynObjAct.position.y + dynObjAct.diameter > dynObjInAct.position.y)
				{
					dynObjAct.position.y = dynObjInAct.position.y - dynObjInAct.diameter;
					dynObjAct.hasLanded = true;
					dynObjInAct.hasLanded = true;

				}
				if (dynObjAct.ptrTandem != NULL)
				{
					dynObjAct.ptrTandem->hasLanded = true;
					dynObjAct.ptrTandem->position.x = dynObjAct.position.x + dynObjAct.tandemDir.x * dynObjAct.diameter;
					dynObjAct.ptrTandem->position.y = dynObjAct.position.y + dynObjAct.tandemDir.y * dynObjAct.diameter;
				}
			}
		}
	}
}

