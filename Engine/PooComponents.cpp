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


}
//graphics
void PooGraphicsComponent::draw(GameObject& obj, DirectX::SpriteBatch& batch)
{
	spritePoo->Draw(batch, { obj.position.x, obj.position.y });
}
//bounds collision
void PooPhysicsComponent::collisionBounds(GameObject& obj, float screenWidth, float screenHeight)
{

}
//resolve bounds collision
void PooPhysicsComponent::resolveBoundsCollision(GameObject& obj, float screenWidth, float screenHeight)
{


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

				if (dynObj_Act.physics->move.x < 0.0f)
				{
					dynObj_Act.physics->collidesType = eCollides::LEFT;
				}
				else if (dynObj_Act.physics->move.x > 0.0f)
				{
					dynObj_Act.physics->collidesType = eCollides::RIGHT;
				}
				else if (dynObj_Act.position.y + dynObj_Act.diameter > dynObj_InAct.position.y)
				{

					dynObj_Act.physics->collidesType = eCollides::BOT;
				}
			}
		}
	}
}
//resolve obj to obj collision
void PooPhysicsComponent::resolveObjCollision(GameObject& obj_Active, GameObject& obj_Inactive)
{

}

