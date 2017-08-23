#include "PooYo.h"


PooYo::PooYo( float X, float Y, Color C )
	:
	position( X, Y ),
	color( C )
{
}

void PooYo::Update( float DeltaTime )
{
	constexpr float speed = 100.f;
	const auto delta = targetPos - position;
	auto dir = Normalize( delta );
	const auto dist = DotProduct( dir, delta );
	
	if(dist > (speed * DeltaTime))
	{
		if( std::fabsf( dir.x ) > std::fabsf( dir.y ) )
		{
			dir.y = 0.f;
		}
		else
		{
			dir.x = 0.f;
		}
		velocity = dir * ( speed * DeltaTime );
		position += velocity;
	}
	else
	{
		velocity = { 0.f, 0.f };
	}
}

Rectf PooYo::GetRect() const
{
	Rectf rect;
	rect.left = position.x;
	rect.top = position.y;
	rect.right = rect.left + 32.f;
	rect.bottom = rect.top + 32.f;

	return rect;
}

void PooYo::SetTargetPos( const Vec2f & TargetPos )
{
	targetPos = TargetPos;
}

void PooYo::SetWayPoint( const Vec2f & Waypoint )
{
	waypoint = Waypoint;
}
