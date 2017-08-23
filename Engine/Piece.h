#pragma once

#include "PooYo.h"
#include <memory>

class Piece
{
	enum class Orientation
	{
		_0, _90, _180, _270
	};
public:
	Piece() = default;
	Piece( class Board &Brd, Color C0, Color C1 );

	void Update( float DeltaTime );

	void MoveLeft( const class Board &Brd );
	void MoveRight( const class Board &Brd );
	void RotateCW( const class Board &Brd );

	std::pair<PooYo, PooYo> Collect();
	void SetWaypoint( const Vec2f &PrimaryPos, const Vec2f &SecondaryPos );
	void SetTarget( const Vec2f &PrimaryPos, const Vec2f &SecondaryPos );

	bool BothHaveSettled();
	bool PrimaryHasReachedTarget()const;
	bool SecondaryHasReachedTarget()const;

public:
	std::unique_ptr<PooYo[]> pPooyos;
	Vec2f waypoint;
	Orientation orientation = Orientation::_0;
};