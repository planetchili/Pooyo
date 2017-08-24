#include "PooComponents.h"
#include "GameObject.h"



using namespace DirectX;

void PooInputComponent::update(GameObject&)
{

}

void PooPhysicsComponent::update(GameObject& obj, float delta)
{
	
	obj.position.y += delta * 40.0f;
}

void PooGraphicsComponent::update(GameObject& obj, Graphics& gfx)
{
	

	auto batch = gfx.MakeSpriteBatch();

	batch.Begin(DirectX::SpriteSortMode_Deferred, gfx.GetStates().NonPremultiplied(), gfx.GetStates().PointClamp());

	spritePoo->Draw(batch, obj.position);

	batch.End();
}

