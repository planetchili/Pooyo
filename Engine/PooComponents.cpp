#include "PooComponents.h"
#include "PooObject.h"



using namespace DirectX;

void PooInputComponent::update(GameObject& obj, Keyboard& kbd)
{
	unsigned char keyCode = kbd.ReadKey().GetCode();
	if (kbd.KeyIsPressed(keyCode))
	{
		switch (keyCode)
		{
		case VK_LEFT:
			obj.position.x += -1.0f * dynamic_cast<PooObject&>(obj).diameter;
			break;
		case VK_RIGHT:
			obj.position.x += 1.0f * dynamic_cast<PooObject&>(obj).diameter;
		}
	}
}

void PooPhysicsComponent::update(GameObject& obj, float delta, Graphics& gfx)
{
	if(!dynamic_cast<PooObject&>(obj).hasLanded)
		obj.position.y += delta * 80.0f;

	if (obj.position.y > gfx.ScreenHeight)
		dynamic_cast<PooObject&>(obj).hasLanded = true;
}

void PooGraphicsComponent::update(GameObject& obj, Graphics& gfx)
{
	

	auto batch = gfx.MakeSpriteBatch();

	batch.Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates().NonPremultiplied(), gfx.GetStates().PointClamp());

	spritePoo->Draw(batch, obj.position);

	batch.End();
}

