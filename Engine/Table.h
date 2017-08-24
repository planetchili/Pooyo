#pragma once

#include "Puyo.h"
#include "Vec2.h"
#include <vector>
#include <unordered_set>

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
	auto GetPositions() const
	{
		return std::make_pair( pos,pos + dir );
	}
	auto GetPlanet() const
	{
		return std::make_pair( planet.GetCopy(),pos );
	}
	auto GetSatellite() const
	{
		return std::make_pair( satellite.GetCopy(),pos + dir );
	}
	Piece& operator++()
	{
		++dir;
		return *this;
	}
	Piece& operator--()
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
	Piece GetCopy() const
	{
		Piece p;
		p.planet = planet.GetCopy();
		p.satellite = satellite.GetCopy();
		p.pos = pos;
		p.dir = dir;
		return p;
	}
	void Draw( Graphics& gfx,const Vec2& table_pos ) const
	{
		auto sb = gfx.MakeSpriteBatch();
		sb.Begin( DirectX::SpriteSortMode_Deferred,
				  gfx.GetStates().NonPremultiplied() );
		planet.Draw( sb,table_pos + Vec2( pos ) * Puyo::GetPuyoSize() );
		satellite.Draw( sb,table_pos + Vec2( pos + dir ) * Puyo::GetPuyoSize() );
		sb.End();
	}
private:
	Puyo planet;
	Puyo satellite;
	Vei2 pos;
	Direction dir = Direction::Down();
};

class Table
{
public:
	Table( int width,int height,Graphics& gfx )
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
		return const_cast<Table*>(this)->At( pos );
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
				const Vei2 tablePos = { x,y };
				At( tablePos ).Draw( sb,pos + Vec2( tablePos ) * Puyo::GetPuyoSize() );
			}
		}
		sb.End();
	}
	bool IsColliding( const Piece& p ) const
	{
		const auto pos = p.GetPositions();
		return !IsInBoard( pos.first ) || !IsInBoard( pos.second ) ||
			!At( pos.first ).IsEmpty() || !At( pos.second ).IsEmpty();
	}
	bool IsResting( const Piece& p ) const
	{
		const auto pos = p.GetPositions();
		return pos.first.y == height - 1 || pos.second.y == height - 1 ||
			!At( pos.first + Vei2{ 0,1 } ).IsEmpty() ||
			!At( pos.second + Vei2{ 0,1 } ).IsEmpty();
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
	void LockPiece( const Piece& p )
	{
		auto pair = p.GetPlanet();
		At( pair.second ) = std::move( pair.first );
		pair = p.GetSatellite();
		At( pair.second ) = std::move( pair.first );
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
				const Vei2 tablePos = { x,y };
				if( filter.count( tablePos ) == 0u )
				{
					At( tablePos ).Draw( sb,pos + Vec2( tablePos ) * Puyo::GetPuyoSize() );
				}
			}
		}
		sb.End();
	}
private:
	class FindDying_
	{
	public:
		FindDying_( const Table& table )
			:
			table( table )
		{}
		PositionSet operator()()
		{
			for( int y = 0; y < table.GetHeight(); y++ )
			{
				for( int x = 0; x < table.GetWidth(); x++ )
				{
					const Vei2 pos = { x,y };
					const Puyo& puyo = table.At( pos );
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
			if( table.IsInBoard( pos ) )
			{
				const Puyo& puyo = table.At( pos );
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
		const Table& table;
		std::vector<std::vector<Vei2>> groups;
		PositionSet visited;
	};

private:	   
	int width;		
	int height;
	std::vector<Puyo> puyos;
};