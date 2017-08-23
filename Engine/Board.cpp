#include "Board.h"
#include <queue>

void Board::Cell::RegisterPooYo( Color C )
{
	color = C;
	SetReserved();
}

Color Board::Cell::UnregisterPooYo()
{
	Color result = Colors::Black;
	std::swap( result, color );
	ClearReserved();
	return result;
}

bool Board::Cell::IsEmpty() const
{
	return color == Colors::Black;
}

void Board::Cell::MakeEmpty()
{
	UnregisterPooYo();
}

bool Board::Cell::IsReserved() const
{
	return reserved;
}

void Board::Cell::SetReserved()
{
	reserved = true;
}

void Board::Cell::ClearReserved()
{
	reserved = false;
}

Board::Board( float PosX, float PosY, int NumColumns, int NumRows )
	:
	boardPos( PosX, PosY ),
	numCols( NumColumns ),
	numRows( NumRows ),
	cells( NumColumns * NumRows )
{}

bool Board::CanSpawn() const
{
	return ( cells[ MakeIndex( { 2, 0 } ) ].IsEmpty() &&
			 cells[ MakeIndex( { 2, 1 } ) ].IsEmpty() );
}

Vec2f Board::GetSpawnPosUpper() const
{
	return ToPixel( { 2, 0 } );
}

Vec2f Board::GetSpawnPosLower() const
{
	return ToPixel( { 2, 1 } );
}

bool Board::CanMoveLeft( const Vec2f & Pos0, const Vec2f & Pos1 ) const
{
	const auto p0 = ToGrid( Pos0 );
	const auto p1 = ToGrid( Pos1 );
	
	if( p0.x > 0 && p1.x > 0 ) 
	{
		if( p0.y == p1.y )  // Pair is horizontal
		{
			const auto curCellGridPos = ToGrid( p0.x < p1.x ? Pos0 : Pos1 );
			return cells[ MakeIndex( curCellGridPos ) - 1 ].IsEmpty();
		}
		else  // Pair is vertical
		{
			const auto topGridPos = ToGrid( Pos0.y < Pos1.y ? Pos0 : Pos1 );
			const auto btmGridPos = ToGrid( Pos0.y < Pos1.y ? Pos1 : Pos0 );

			return
				cells[ MakeIndex( topGridPos ) - 1 ].IsEmpty() &&
				cells[ MakeIndex( btmGridPos ) - 1 ].IsEmpty();
		}
	}

	return false;
}

bool Board::CanMoveRight( const Vec2f & Pos0, const Vec2f & Pos1 ) const
{
	const auto p0 = ToGrid( Pos0 );
	const auto p1 = ToGrid( Pos1 );

	if( p0.x < numCols -1 && p1.x < numCols - 1 )
	{
		if( p0.y == p1.y )  // Pair is horizontal
		{
			const auto curCellGridPos = ToGrid( p0.x > p1.x ? Pos0 : Pos1 );
			return cells[ MakeIndex( curCellGridPos ) + 1 ].IsEmpty();
		}
		else  // Pair is vertical
		{
			const auto topGridPos = ToGrid( Pos0.y < Pos1.y ? Pos0 : Pos1 );
			const auto btmGridPos = ToGrid( Pos0.y < Pos1.y ? Pos1 : Pos0 );

			return
				cells[ MakeIndex( topGridPos ) + 1 ].IsEmpty() &&
				cells[ MakeIndex( btmGridPos ) + 1 ].IsEmpty();
		}
	}

	return false;
}

bool Board::CanRotate( const Vec2f & Pos0, const Vec2f & Pos1 ) const
{
	// Check orientation using Pos0 as pivot
	const auto cellDiff = ToGrid( Pos1 ) - ToGrid( Pos0 );

	if( cellDiff.x < 0 )		// Pos1 is left of pivot
	{
		const auto top = MakeIndex( ToGrid( ToUpper( Pos0 ) ) );
		return cells[top].IsEmpty();
	}
	else if( cellDiff.x > 0 )	// Pos1 is right of pivot
	{
		const auto bottom = MakeIndex( ToGrid( ToLower( Pos0 ) ) );
		return cells[ bottom ].IsEmpty();
	}
	else
	{
		if( cellDiff.y < 0 )		// Pos1 is above pivot
		{
			const auto right = MakeIndex( ToGrid( ToRight( Pos0 ) ) );
			return cells[ right ].IsEmpty();
		}
		else						// Pos1 is below pivot
		{
			const auto left = MakeIndex( ToGrid( ToLeft( Pos0 ) ) );
			return cells[ left ].IsEmpty();
		}
	}
}

Vec2f Board::ToLeft( const Vec2f & Pos ) const
{
	const auto pos = ToGrid( Pos );
	return ToPixel( { pos.x - 1, pos.y } );
}

Vec2f Board::ToUpper( const Vec2f & Pos ) const
{
	const auto pos = ToGrid( Pos );
	return ToPixel( { pos.x, pos.y - 1 } );
}

Vec2f Board::ToRight( const Vec2f & Pos ) const
{
	const auto pos = ToGrid( Pos );
	return ToPixel( { pos.x + 1, pos.y } );
}

Vec2f Board::ToLower( const Vec2f & Pos ) const
{
	const auto pos = ToGrid( Pos );
	return ToPixel( { pos.x, pos.y + 1 } );
}

void Board::CheckForColorChains( const Vec2f Pos )
{
	auto cellPos = ToGrid( Pos );
	const auto startIndex = MakeIndex( cellPos );
	const auto colorToSearchFor = cells[ startIndex ].color;

	int nuisancecount = 0;
	std::queue<int> toVisit;
	std::vector<int> visited;
	toVisit.push( startIndex );
	
	auto IndexCheck = [ this, colorToSearchFor, &toVisit, &visited ]( int X, int Y )
	{
		const int index = X + ( Y * numCols );
		auto it = std::find( visited.begin(), visited.end(), index );

		if(it == visited.end())		// If it == end, then not in visited list
		{
			if(	X >= 0 && X < numCols &&
				Y >= 0 && Y < numRows )
			{
				if( cells[ index ].color == colorToSearchFor )
				{
					toVisit.push( index );
				}
			}
		}
	};
	auto BFSColorChain = [ this, &IndexCheck, &toVisit, &visited ]()
	{
		while( !toVisit.empty() )
		{
			int curIdx = toVisit.front();
			toVisit.pop();
			visited.push_back( curIdx );
			const Vec2i cellPos = { curIdx % numCols, curIdx / numCols };

			// Check above
			IndexCheck( cellPos.x, cellPos.y - 1 );
			// Check right
			IndexCheck( cellPos.x + 1, cellPos.y );
			// Check down
			IndexCheck( cellPos.x, cellPos.y + 1 );
			// Check left
			IndexCheck( cellPos.x + 1, cellPos.y );
		}

		colorchainIndices = std::move( visited );
	};

	auto NuisanceCheck = [ this, &nuisancecount ]( int X, int Y )
	{
		if( X >= 0 && X <= numCols &&
			Y >= 0 && Y <= numRows )
		{
			const int index = X + ( Y * numCols );
			if( cells[ index ].color == Colors::Black )
			{
				++nuisancecount;
				nuisancesIndices.push_back( index );
			}
		}
	};
	auto BFSNuisanceChain = [ this, &visited, &NuisanceCheck ]()
	{
		if( visited.size() < 4 ) return;

		for( size_t i = 0; i < visited.size(); ++i )
		{
			const int curIdx = visited[ i ];
			const int x = curIdx % numCols;
			const int y = curIdx / numCols;
			
			// Check above
			NuisanceCheck( x, y - 1 );
			// Check right
			NuisanceCheck( x + 1, y );
			// Check below
			NuisanceCheck( x, y + 1 );
			// Check left
			NuisanceCheck( x - 1, y );
		}
	};

	BFSColorChain();
	BFSNuisanceChain();
}

void Board::RemoveChains()
{
	for( const auto index : colorchainIndices )
	{
		cells[ index ].MakeEmpty();
	}
	for( const auto index : nuisancesIndices )
	{
		cells[ index ].MakeEmpty();
	}
}

Vec2f Board::LastAvailableCell( const Vec2f & Pos ) const
{
	Vec2i cellPos = ToGrid( Pos );
	while( cellPos.y + 1 < numRows )
	{
		const int index = MakeIndex( { cellPos.x, cellPos.y + 1 } );
		const auto &cell = cells[ index ];

		if( !cell.IsEmpty() || cell.IsReserved() )
		{
			return ToPixel( cellPos );
		}
		++cellPos.y;
	}

	return ToPixel( cellPos );
}

bool Board::IsInLastAvailableCell( const Vec2f & Pos ) const
{
	return ToGrid( Pos ).y == ToGrid( LastAvailableCell( Pos ) ).y;
}

void Board::ReserveCell( const Vec2f & Pos )
{
	cells[ MakeIndex( ToGrid( Pos ) ) ].SetReserved();
}

void Board::HandleCellRegistry( const Rectf & PooYoRect )
{
	const Vec2i lefttop = ToGrid( { PooYoRect.left, PooYoRect.top } );
	const Vec2i rightbtm = ToGrid( { PooYoRect.right, PooYoRect.bottom } );
	
	if( lefttop.x != rightbtm.x )
	{
		const int rightIdx = MakeIndex( rightbtm );
		const int leftIdx = MakeIndex( lefttop );

		if( cells[ rightIdx ].IsEmpty() &&
			!cells[ leftIdx ].IsEmpty() )
		{
			// Handle moving right
			auto color = cells[ leftIdx ].UnregisterPooYo();
			cells[ rightIdx ].RegisterPooYo( color );
		}
		else if( !cells[ rightIdx ].IsEmpty() &&
				 cells[ leftIdx ].IsEmpty() )
		{
			// Handle moving left
			auto color = cells[ rightIdx ].UnregisterPooYo();
			cells[ leftIdx ].RegisterPooYo( color );
		}
	}

	if( lefttop.y != rightbtm.y )
	{
		// Handle moving down
		const int prevIdx = MakeIndex( lefttop );
		const int nextIdx = MakeIndex( rightbtm );

		auto color = cells[ prevIdx ].UnregisterPooYo();
		cells[ nextIdx ].RegisterPooYo( color );
	}
}

Vec2i Board::ToGrid( const Vec2f & Pos ) const
{
	const auto pos = Pos * ( 1.f / static_cast< float >( cellSize ) );
	return Vec2i(
		static_cast< int >( pos.x ),
		static_cast< int >( pos.y )
	);
}

Vec2f Board::ToPixel( const Vec2i & Pos ) const
{
	const auto pos = Pos * cellSize;
	return Vec2f(
		static_cast< float >( pos.x ),
		static_cast< float >( pos.y )
	);
}

int Board::MakeIndex( const Vec2i & GridPos ) const
{
	return GridPos.x + ( GridPos.y * numCols );
}
