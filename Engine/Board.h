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
						TryAddToGroup( pos,puyo );
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
				if( puyo == type )
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