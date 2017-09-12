#include "PooComponents.h"
#include "PooObject.h"



using namespace DirectX;

//input
void PooInputComponent::update(GameObject& obj, Keyboard& kbd)
{

}
//physics
void PooPhysicsComponent::movement(GameObject& obj, float delta)
{
	PooObject& dynObj_Act = dynamic_cast<PooObject&>(obj);
	if (!dynObj_Act.hasLanded)
	{
		dynObj_Act.position.y += dynObj_Act.physics->move.y * dynObj_Act.soloSpeed *delta;
		
	}

}
//graphics
void PooGraphicsComponent::draw(GameObject& obj, DirectX::SpriteBatch& batch)
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
	PooObject& dynObj_Act = dynamic_cast<PooObject&>(obj);

	switch (dynObj_Act.physics->collidesType)
	{
	case eCollides::BOUNDS_LEFT:
		dynObj_Act.position.x = 0.0f;
		break;
	case eCollides::BOUNDS_RIGHT:
		dynObj_Act.position.x = screenWidth - dynObj_Act.diameter;
		break;
	case eCollides::BOUNDS_BOT:
		dynObj_Act.hasLanded = true;
		dynObj_Act.position.y = screenHeight - dynObj_Act.diameter;
		break;
	}
	dynObj_Act.physics->collidesType = eCollides::DFLT;
}
//pooyo to pooyo collision
void PooPhysicsComponent::collisionObj(GameObject& obj_Active, GameObject& obj_Inactive)
{
	
	PooObject& dynObj_Act = dynamic_cast<PooObject&>(obj_Active);
	PooObject& dynObj_InAct = dynamic_cast<PooObject&>(obj_Inactive);

	if (&dynObj_Act != &dynObj_InAct)
	{
		if (dynObj_InAct.hasLanded)
		{
			Vector2 diff = dynObj_Act.position - dynObj_InAct.position;

			if (diff.LengthSquared() < pow(dynObj_Act.diameter, 2.0f))
			{
				dynObj_Act.physics->collidesType = eCollides::BOT;//initial set to bot unless tandem player controller overrides
				dynObj_Act.hasCollided = true;
			}
		}
	}
}
//resolve obj to obj collision
void PooPhysicsComponent::resolveObjCollision(GameObject& obj_Active, GameObject& obj_Inactive)
{
	PooObject& dynObj_Act = dynamic_cast<PooObject&>(obj_Active);
	PooObject& dynObj_InAct = dynamic_cast<PooObject&>(obj_Inactive);

	switch (dynObj_Act.physics->collidesType)
	{
	case eCollides::LEFT:
		dynObj_Act.position.x = dynObj_InAct.position.x + dynObj_InAct.diameter;
		break;
	case eCollides::RIGHT:
		dynObj_Act.position.x = dynObj_InAct.position.x - dynObj_InAct.diameter;
		break;
	case eCollides::BOT:
		dynObj_Act.position.y = dynObj_InAct.position.y - dynObj_InAct.diameter;
		dynObj_Act.hasLanded = true;
		dynObj_InAct.hasLanded = true;
		break;
	}
	dynObj_Act.physics->collidesType = eCollides::DFLT;
	dynObj_Act.hasCollided = false;
}

