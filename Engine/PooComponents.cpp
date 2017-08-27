#include "PooComponents.h"
#include "PooObject.h"



using namespace DirectX;

void PooInputComponent::update(GameObject& obj, Keyboard& kbd, float delta)
{
	unsigned char keyCode = kbd.ReadKey().GetCode();
	PooObject& pooObj = dynamic_cast<PooObject&>(obj);

	if (kbd.KeyIsPressed(keyCode))
	{
		switch (keyCode)
		{
		case 'A':
			pooObj.position.x += -1.0f * pooObj.diameter;
			break;
		case 'D':
			pooObj.position.x += 1.0f * pooObj.diameter;
			break;
		}
	}
	if(kbd.KeyIsPressed('S'))
		pooObj.position.y += delta * pooObj.speed * 10.0f;

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
		dynamic_cast<PooObject&>(obj).hasLanded = true;
}

void PooGraphicsComponent::update(GameObject& obj, Graphics& gfx)
{
	

	auto batch = gfx.MakeSpriteBatch();

	batch.Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates().NonPremultiplied(), gfx.GetStates().PointClamp());

	spritePoo->Draw(batch, obj.position);

	batch.End();
}

void PooCollisionComponent::update(GameObject* obj_A, GameObject& obj_B)
{
	if (obj_A != &obj_B)
	{

		XMFLOAT2 diff = { obj_A->position.x - obj_B.position.x, obj_A->position.y - obj_B.position.y };
		float magnitude = sqrt(diff.x * diff.x + diff.y * diff.y);
		if (magnitude < obj_A->diameter)
		{
			dynamic_cast<PooObject*>(obj_A)->hasLanded = true;
			dynamic_cast<PooObject&>(obj_B).hasLanded = true;
			if (obj_A->position.y < obj_B.position.y)
				obj_A->position.y = obj_B.position.y - obj_B.diameter;
		}
	}
}

