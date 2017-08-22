#include "PooComponents.h"
#include "GameObject.h"
#include "Sprite.h"


using namespace DirectX;

void PooInputComponent::update(GameObject&)
{

}

void PooPhysicsComponent::update(GameObject&)
{

}

void PooGraphicsComponent::update(GameObject& obj, SpriteBatch& sb)
{
	spritePoo->Draw(sb, { 10.0f, 10.0f });
}

