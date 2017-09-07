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

}
//resolve obj to obj collision
void PooPhysicsComponent::resolveObjCollision(GameObject& obj_Active, GameObject& obj_Inactive)
{

}

