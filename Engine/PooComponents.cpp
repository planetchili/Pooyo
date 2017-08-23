#include "PooComponents.h"
#include "GameObject.h"



using namespace DirectX;

void PooInputComponent::update(GameObject&)
{

}

void PooPhysicsComponent::update(GameObject&)
{

}

void PooGraphicsComponent::update(GameObject& obj, Graphics& gfx)
{
	auto batch = gfx.MakeSpriteBatch();

	batch.Begin(DirectX::SpriteSortMode_Deferred,
		gfx.GetStates().NonPremultiplied(),
		gfx.GetStates().PointClamp());

	spritePoo->Draw(batch, { 10.0f, 10.0f });

	batch.End();
}

