#pragma once

#include "Rect.h"
#include "Colors.h"

class PooYo
{
public:
	PooYo() = default;
	PooYo( float X, float Y, Color C );

	void Update( float DeltaTime );

	Rectf GetRect()const;
	void SetTargetPos( const Vec2f &TargetPos );
	void SetWayPoint( const Vec2f &Waypoint );
public:
	Vec2f position, velocity, targetPos, waypoint, prevPosition;
	Color color;
};
