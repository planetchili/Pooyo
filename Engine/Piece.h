#pragma once

#include "Board.h"

class Piece
{
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
		const auto temp = dir.x;
		dir.x = -dir.y;
		dir.y = temp;
		return *this;
	}
	Piece& CCWRotate()
	{
		const auto temp = dir.x;
		dir.x = dir.y;
		dir.y = -temp;
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
		Piece piece;
		piece.planet = planet.Clone();
		piece.satellite = satellite.Clone();
		piece.pos = pos;
		piece.dir = dir;
		return piece;
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
	Vei2 dir = { 0,1 };
};