#include "Piece.h"
#include "Board.h"

Piece::Piece( class Board &Brd, Color C0, Color C1 )
	:
	pPooyos( std::make_unique<PooYo[]>( 2 ) )
{
	auto InitPooYo = [&Brd]( PooYo &Pooyo, const Vec2f &Pos, Color C )
	{
		Pooyo = PooYo( Pos.x, Pos.y, C );

		Brd.RegisterWithCell( Pos, C );

		const auto target = Brd.LastAvailableCell( Pos );
		Brd.ReserveCell( target );
		Pooyo.SetTargetPos( target );
	};

	InitPooYo( pPooyos[ 0 ], Brd.GetSpawnPosLower(), C0 );
	InitPooYo( pPooyos[ 1 ], Brd.GetSpawnPosUpper(), C1 );
}

void Piece::Update( float DeltaTime )
{
	auto UpdateIf = [ DeltaTime ]( bool NotSettled, PooYo &Pooyo )
	{
		if( NotSettled )
		{
			Pooyo.Update( DeltaTime );
		}
	};

	UpdateIf( !PrimaryHasReachedTarget(), pPooyos[ 0 ] );
	UpdateIf( !SecondaryHasReachedTarget(), pPooyos[ 1 ] );
}

void Piece::MoveLeft( const Board & Brd )
{
	if( Brd.CanMoveLeft( pPooyos[ 0 ].position, pPooyos[ 1 ].position ) )
	{
		auto CalcWaypoint = [ this, &Brd ]( const PooYo &Pooyo )
		{
			auto newpos = Brd.ToLeft( Pooyo.position );
			newpos.y = Pooyo.position.y;
			return newpos;
		};

		SetWaypoint(
			CalcWaypoint( pPooyos[ 0 ] ),
			CalcWaypoint( pPooyos[ 1 ] ) );
	}
}

void Piece::MoveRight( const Board & Brd )
{
	if( Brd.CanMoveRight( pPooyos[ 0 ].position, pPooyos[ 1 ].position ) )
	{
		auto CalcWaypoint = [ this, &Brd ]( const PooYo &Pooyo )
		{
			auto newpos = Brd.ToRight( Pooyo.position );
			newpos.y = Pooyo.position.y;
			return newpos;
		};

		SetWaypoint(
			CalcWaypoint( pPooyos[ 0 ] ),
			CalcWaypoint( pPooyos[ 1 ] ) );
	}
}

void Piece::RotateCW( const Board & Brd )
{
	if( Brd.CanRotate( pPooyos[ 0 ].position, pPooyos[ 1 ].position ) )
	{
		switch( orientation )
		{
			case Orientation::_0:
				pPooyos[ 1 ].position = Brd.ToUpper( pPooyos[ 0 ].position );
				orientation = Orientation::_90;
				break;
			case Orientation::_90:
				pPooyos[ 1 ].position = Brd.ToRight( pPooyos[ 0 ].position );
				orientation = Orientation::_180;
				break;
			case Orientation::_180:
				pPooyos[ 1 ].position = Brd.ToLower( pPooyos[ 0 ].position );
				orientation = Orientation::_270;
				break;
			case Orientation::_270:
				pPooyos[ 1 ].position = Brd.ToLeft( pPooyos[ 0 ].position );
				orientation = Orientation::_0;
				break;
		}
	}	
}

std::pair<PooYo, PooYo> Piece::Collect()
{
	return std::pair<PooYo, PooYo>( pPooyos[ 0 ], pPooyos[ 1 ] );
}

void Piece::SetWaypoint( const Vec2f &PrimaryPos, const Vec2f &SecondaryPos )
{
	pPooyos[ 0 ].SetWayPoint( PrimaryPos );
	pPooyos[ 1 ].SetWayPoint( SecondaryPos );

	SetTarget( PrimaryPos, SecondaryPos );
}

void Piece::SetTarget( const Vec2f & PrimaryPos, const Vec2f & SecondaryPos )
{
	pPooyos[ 0 ].SetTargetPos( PrimaryPos );
	pPooyos[ 1 ].SetTargetPos( SecondaryPos );
}

bool Piece::BothHaveSettled()
{
	return PrimaryHasReachedTarget() && SecondaryHasReachedTarget();
}

bool Piece::PrimaryHasReachedTarget() const
{
	return static_cast< int >( pPooyos[ 0 ].velocity.y ) == 0;
}

bool Piece::SecondaryHasReachedTarget() const
{
	return static_cast< int >( pPooyos[ 1 ].velocity.y ) == 0;
}
