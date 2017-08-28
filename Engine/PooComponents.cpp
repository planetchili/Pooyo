#include "PooComponents.h"
#include "PooObject.h"



using namespace DirectX;

void PooInputComponent::update(GameObject& obj, Keyboard& kbd, float delta)
{
	unsigned char keyCode = kbd.ReadKey().GetCode();
	PooObject& pooObj = dynamic_cast<PooObject&>(obj);
	pooObj.moveDir = PooObject::Dir(0.0f, 1.0f );

	if (kbd.KeyIsPressed(keyCode))
	{
		switch (keyCode)
		{
		case 'A':
			pooObj.moveDir = PooObject::Dir(-1.0f, 0.0f );
			pooObj.position.x += -1.0f * pooObj.diameter;
			break;
		case 'D':
			pooObj.moveDir = PooObject::Dir(1.0f, 0.0f );
			pooObj.position.x += 1.0f * pooObj.diameter;
			break;
		}
	}
	if (kbd.KeyIsPressed('S'))
	{
		pooObj.moveDir = PooObject::Dir(0.0f, 1.0f );
		pooObj.position.y += delta * pooObj.speed * 20.0f;
	}

	if (dynamic_cast<PooObject&>(obj).ptrTandem != NULL)
	{
		pooObj.ptrTandem->position.x = obj.position.x + pooObj.tandemDir.x * pooObj.diameter;
		pooObj.ptrTandem->position.y = obj.position.y + pooObj.tandemDir.y * pooObj.diameter;
	}


	
}

void PooPhysicsComponent::update(GameObject& obj, float delta, Graphics& gfx)
{
	if(!dynamic_cast<PooObject&>(obj).hasLanded)
		obj.position.y += delta * obj.speed;

	if (obj.position.y + obj.diameter > gfx.ScreenHeight)
	{
		dynamic_cast<PooObject&>(obj).hasLanded = true;
		obj.position.y = gfx.ScreenHeight - obj.diameter;
	}

}

void PooGraphicsComponent::update(GameObject& obj, DirectX::SpriteBatch& batch)
{
	spritePoo->Draw(batch, obj.position);
}

void PooCollisionComponent::update(GameObject& obj_Active, GameObject& obj_Inactive)
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
				
				if (dynObjAct.moveDir.x < 0.0f)
				{
					dynObjAct.position.x = dynObjInAct.position.x + dynObjInAct.diameter;
				}
				else if (dynObjAct.moveDir.x > 0.0f)
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
					dynObjAct.ptrTandem->position.x = dynObjAct.position.x + dynObjAct.tandemDir.x * dynObjAct.diameter;
					//	dynObjAct.ptrTandem->position.y = dynObjAct.position.y + dynObjAct.tandemDir.y * dynObjAct.diameter;
				}
			}
		}
	}
}

