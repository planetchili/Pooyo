#include "TandemComponents.h"
#include "PooObject.h"



using namespace DirectX;

void TandemInptCmpt::update(GameObject& obj, Keyboard& kbd, float delta)
{
	unsigned char keyCode = kbd.ReadKey().GetCode();
	PooObject& pooObj = dynamic_cast<PooObject&>(obj);
	pooObj.move = { 0.0f, pooObj.speed};

	if (kbd.KeyIsPressed(keyCode))
	{
		switch (keyCode)
		{
		case 'A':
			pooObj.move = { -1.0f, 0.0f };
			pooObj.position.x += -1.0f * pooObj.diameter;
			break;
		case 'D':
			pooObj.move = { 1.0f, 0.0f };
			pooObj.position.x += 1.0f * pooObj.diameter;
			break;
		}
	}
	if (kbd.KeyIsPressed('S'))
	{
		pooObj.move = { 0.0f, 1.0f };
		pooObj.position.y += delta * pooObj.speed * 20.0f;
	}

	if (dynamic_cast<PooObject&>(obj).ptrTandem != NULL)
	{
		pooObj.ptrTandem->position.x = obj.position.x + pooObj.tandemDir.x * pooObj.diameter;
		pooObj.ptrTandem->position.y = obj.position.y + pooObj.tandemDir.y * pooObj.diameter;
	}



}