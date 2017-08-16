#pragma once

#include "Puyo.h"
#include "Vec2.h"
#include <vector>

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
				At( tablePos ).Draw( sb,pos + Vec2( tablePos ) * puyo_size );
			}
		}
		sb.End();
	}

private:	   
	int width;		
	int height;
	float puyo_size = 32.0f;
	std::vector<Puyo> puyos;
};