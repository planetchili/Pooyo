#pragma once

#include "Components.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;
class GameObject
{
public:
	//struct Vec2
	//{
	//	float x;
	//	float y;
	//	Vec2() { x = 0.0f; y = 0.0f; }
	//	Vec2(float x, float y)
	//	{
	//		this->x = x; this->y = y;
	//	}
	//	Vec2& operator+=(Vec2 const& rhs)
	//	{
	//		this->x += rhs.x;
	//		this->y += rhs.y;
	//		return *this;
	//	}
	//	Vec2 operator*(float const& rhs)
	//	{
	//		return Vec2(this->x * rhs, this->y *= rhs);
	//	}
	//};

	float diameter = 12.0f * 4.0f;
	float speed = 20.0f;
	//Vec2 position;
	Vector2 position;

	GameObject(ComponentInput*, ComponentPhysics*, ComponentGraphics*);
	virtual void update(Keyboard&, float);//input
	virtual void update(Graphics&, float);//physics
	virtual void update(GameObject&);//collision
	virtual void update(DirectX::SpriteBatch&);//graphics
	
public:
	ComponentInput* input;
	ComponentPhysics* physics;
	ComponentGraphics* graphics;
};

