#pragma once

#include "Puyo.h"
#include "Vec2.h"
#include <vector>
#include <unordered_set>


class Board
{
public:
	Board( int width,int height,Graphics& gfx )
		:
		width( width ),
		height( height ),
		puyos( width * height )
	{
		Puyo::InitSprites( gfx );
	}
	void Move( const Vei2& src,const Vei2& dir )
	{
		const auto dest = src + dir;
		// make sure vector represent unit direction
		assert( "dir must be unit 4-direction" && std::abs( dir.x ) + std::abs( dir.y ) == 1 );
		// make sure destination is empty
		assert( "destination location must be empty" && At( dest ).IsEmpty() );

		At( dest ) = std::move( At( src ) );
	}
	Puyo& At( const Vei2& pos )
	{
		assert( IsInBoard( pos ) );
		return puyos[pos.x + pos.y * width];
	}
	const Puyo& At( const Vei2& pos ) const
	{
		return const_cast<Board*>(this)->At( pos );
	}
	bool IsInBoard( const Vei2& pos ) const
	{
		return pos.x >= 0 && pos.x < width &&
			pos.y >= 0 && pos.y < height;
	}
	void Draw( Graphics& gfx,const Vec2& pos ) const
	{
		auto sb = gfx.MakeSpriteBatch();
		sb.Begin( DirectX::SpriteSortMode_Deferred,
				  gfx.GetStates().NonPremultiplied() );
		for( int y = 0; y < height; y++ )
		{
			for( int x = 0; x < width; x++ )
			{
				const Vei2 boardPos = { x,y };
				At( boardPos ).Draw( sb,pos + Vec2( boardPos ) * Puyo::GetPuyoSize() );
			}
		}
		sb.End();
	}
	int GetWidth() const
	{
		return width;
	}
	int GetHeight() const
	{
		return height;
	}
	float GetScreenWidth() const
	{
		return float( GetWidth() ) * Puyo::GetPuyoSize();
	}
	float GetScreenHeight() const
	{
		return float( GetHeight() ) * Puyo::GetPuyoSize();
	}
	bool HasGaps() const
	{
		for( int x = 0; x < width; x++ )
		{
			bool puyoEncountered = false;
			for( int y = 0; y < height; y++ )
			{
				if( At( { x,y } ).IsEmpty() )
				{
					if( puyoEncountered )
					{
						return true;
					}
				}
				else
				{
					puyoEncountered = true;
				}
			}
		}
		return false;
	}
	bool DoFall()
	{
		bool fallHappened = false;
		for( int x = 0; x < width; x++ )
		{
			bool emptyEncountered = false;
			for( int y = height - 1; y >= 0; y-- )
			{
				const Vei2 pos = { x,y };
				if( !At( pos ).IsEmpty() )
				{
					if( emptyEncountered )
					{
						At( pos + Vei2{ 0,1 } ) = std::move( At( pos ) );
						fallHappened = true;
					}
				}
				else
				{
					emptyEncountered = true;
				}
			}
		}
		return fallHappened;
	}

	typedef std::unordered_set<Vei2,Vei2Hasher> PositionSet;
	PositionSet FindDying() const
	{
		return FindDying_{ *this }();
	}
	void DestroyDying( const PositionSet& dying )
	{
		for( const auto& pos : dying )
		{
			At( pos ) = {};
		}
	}
	// draws only the puyos not in the filter seet
	void Draw( Graphics& gfx,const Vec2& pos,const PositionSet& filter ) const
	{
		auto sb = gfx.MakeSpriteBatch();
		sb.Begin( DirectX::SpriteSortMode_Deferred,
				  gfx.GetStates().NonPremultiplied() );
		for( int y = 0; y < height; y++ )
		{
			for( int x = 0; x < width; x++ )
			{
				const Vei2 boardPos = { x,y };
				if( filter.count( boardPos ) == 0u )
				{
					At( boardPos ).Draw( sb,pos + Vec2( boardPos ) * Puyo::GetPuyoSize() );
				}
			}
		}
		sb.End();
	}
private:
	class FindDying_
	{
	public:
		FindDying_( const Board& board )
			:
			board( board )
		{}
		PositionSet operator()()
		{
			for( int y = 0; y < board.GetHeight(); y++ )
			{
				for( int x = 0; x < board.GetWidth(); x++ )
				{
					const Vei2 pos = { x,y };
					const Puyo& puyo = board.At( pos );
					// initially sceen puyo to make sure it constitutes new group
					if( puyo.IsColor() && visited.count( pos ) == 0u )
					{
						// create new group and begin recursive group walk
						groups.emplace_back();
						TryAddToGroup( { x,y },puyo.GetType() );
					}
				}
			}
			
			// filter groups to keep only 4 or greater size groups
			// put them all in set of dying puyo positions
			PositionSet dying;
			for( auto& g : groups )
			{
				if( g.size() > 3 )
				{
					dying.insert( g.begin(),g.end() );
				}
			}

			return dying;
		}
	private:
		void TryAddToGroup( const Vei2& pos,Puyo::Type type )
		{
			if( board.IsInBoard( pos ) )
			{
				const Puyo& puyo = board.At( pos );
				if( puyo.GetType() == type )
				{
					// try to insert
					// if succeed (not already exists in set), add to current
					// group and recurse on neighbors
					if( visited.insert( pos ).second )
					{
						groups.back().push_back( pos );
						TryAddToGroup( pos + Vei2{ 1,0 },type );
						TryAddToGroup( pos + Vei2{ -1,0 },type );
						TryAddToGroup( pos + Vei2{ 0,1 },type );
						TryAddToGroup( pos + Vei2{ 0,-1 },type );
					}
				}
			}
		}
	private:
		const Board& board;
		std::vector<std::vector<Vei2>> groups;
		PositionSet visited;
	};

private:	   
	int width;		
	int height;
	std::vector<Puyo> puyos;
};

class Piece
{
private:
	class Direction
	{
	public:
		operator const Vei2&() const
		{
			return dir;
		}
		static Direction Up()
		{
			return{ 0,-1 };
		}
		static Direction Down()
		{
			return{ 0,1 };
		}
		static Direction Left()
		{
			return{ -1,0 };
		}
		static Direction Right()
		{
			return{ 1,0 };
		}
		Direction& operator++()
		{
			const int temp = dir.x;
			dir.x = -dir.y;
			dir.y = temp;
			return *this;
		}
		Direction& operator--()
		{
			const int temp = dir.x;
			dir.x = dir.y;
			dir.y = -temp;
			return *this;
		}
	private:
		Direction( int x,int y )
			:
			dir( x,y )
		{}
	private:
		Vei2 dir = { 0,0 };
	};
public:
	Piece() = default;
	Piece( const Vei2& pos,Puyo::Type planet,Puyo::Type satellite )
		:
		pos( pos ),
		planet( planet ),
		satellite( satellite )
	{}
	Piece& CWRotate()
	{
		++dir;
		return *this;
	}
	Piece& CCWRotate()
	{
		--dir;
		return *this;
	}
	Piece& PushLeft()
	{
		pos += {-1,0 };
		return *this;
	}
	Piece& PushRight()
	{
		pos += {1,0 };
		return *this;
	}
	Piece& Drop()
	{
		pos += { 0,1 };
		return *this;
	}
	Piece Clone() const
	{
		Piece p;
		p.planet = planet.Clone();
		p.satellite = satellite.Clone();
		p.pos = pos;
		p.dir = dir;
		return p;
	}
	void LockInto( Board& b )
	{
		b.At( pos ) = std::move( planet );
		b.At( pos + dir ) = std::move( satellite );
	}
	bool IsCollidingIn( const Board& b ) const
	{
		const auto satpos = pos + dir;
		return !b.IsInBoard( pos ) || !b.IsInBoard( satpos ) ||
			!b.At( pos ).IsEmpty() || !b.At( satpos ).IsEmpty();
	}
	bool IsRestingIn( const Board& b ) const
	{
		const auto satpos = pos + dir;
		return pos.y == b.GetHeight() - 1 || satpos.y == b.GetHeight() - 1 ||
			!b.At( pos + Vei2{ 0,1 } ).IsEmpty() ||
			!b.At( satpos + Vei2{ 0,1 } ).IsEmpty();
	}
	void Draw( Graphics& gfx,const Vec2& board_pos ) const
	{
		auto sb = gfx.MakeSpriteBatch();
		sb.Begin( DirectX::SpriteSortMode_Deferred,
				  gfx.GetStates().NonPremultiplied() );
		planet.Draw( sb,board_pos + Vec2( pos ) * Puyo::GetPuyoSize() );
		satellite.Draw( sb,board_pos + Vec2( pos + dir ) * Puyo::GetPuyoSize() );
		sb.End();
	}
private:
	Puyo planet;
	Puyo satellite;
	Vei2 pos;
	Direction dir = Direction::Down();
};