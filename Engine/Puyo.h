#pragma once

#include "Sprite.h"
#include "Graphics.h"

class Puyo
{
public:
	enum class Type
	{
		Red,
		Green,
		Blue,
		Yellow,
		Jama,
		Empty
	};
public:
	Puyo() = default;
	Puyo( Type type )
		:
		type( type )
	{}
	void Draw( DirectX::SpriteBatch& sb,const DirectX::XMFLOAT2& pos ) const
	{
		const Sprite* const pSprite = GetSprite( type );
		if( pSprite != nullptr )
		{
			pSprite->Draw( sb,pos );
		}
	}
	static void InitSprites( Graphics& gfx )
	{
		if( !pRedSprite )
		{
			pRedSprite.reset(		new Sprite( gfx.MakeSprite( L"Images\\puyo.png",{ 0,0,31,31 },1.0f,{ 16.0f,16.0f } ) ) );
			pGreenSprite.reset(		new Sprite( gfx.MakeSprite( L"Images\\puyo.png",{ 0,32,31,63 },1.0f,{ 16.0f,16.0f } ) ) );
			pBlueSprite.reset(		new Sprite( gfx.MakeSprite( L"Images\\puyo.png",{ 0,64,31,95 },1.0f,{ 16.0f,16.0f } ) ) );
			pYellowSprite.reset(	new Sprite( gfx.MakeSprite( L"Images\\puyo.png",{ 0,96,31,127 },1.0f,{ 16.0f,16.0f } ) ) );
			pJamaSprite.reset(		new Sprite( gfx.MakeSprite( L"Images\\puyo.png",{ 0,128,31,159 },1.0f,{ 16.0f,16.0f } ) ) );
		}
	}
	bool IsEmpty() const
	{
		return type == Type::Empty;
	}
	bool IsJama() const
	{
		return type == Type::Jama;
	}
	bool IsColor() const
	{
		return !(IsEmpty() || IsJama());
	}
private:
	static const Sprite* GetSprite( Type type )
	{
		assert( "puyo sprites not initialized!" || pRedSprite );
		switch( type )
		{
		case Type::Red:
			return pRedSprite.get();
		case Type::Green:
			return pGreenSprite.get();
		case Type::Blue:
			return pBlueSprite.get();
		case Type::Yellow:
			return pYellowSprite.get();
		case Type::Jama:
			return pJamaSprite.get();
		case Type::Empty:
			return nullptr;
		}
		assert( "bad puyo type" && false );
		return nullptr;
	}
	static std::unique_ptr<Sprite> pRedSprite;
	static std::unique_ptr<Sprite> pGreenSprite;
	static std::unique_ptr<Sprite> pBlueSprite;
	static std::unique_ptr<Sprite> pYellowSprite;
	static std::unique_ptr<Sprite> pJamaSprite;
private:
	Type type = Type::Empty;
};