#pragma once

#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <string>
#include "TextureCodex.h"
#include <DirectXMath.h>
#include <SpriteBatch.h>

class Sprite
{
public:
	Sprite( TextureCodex::TexturePtr& pTex,const RECT& srcRect,
			float scale,const DirectX::XMFLOAT2& org )
		:
		srcRect( srcRect ),
		scale( scale ),
		pTex( pTex )
	{
		// test rect against texture dimensions
		{
			Microsoft::WRL::ComPtr<ID3D11Texture2D> pTex2d;
			pTex->GetResource( (ID3D11Resource**)pTex2d.GetAddressOf() );
			D3D11_TEXTURE2D_DESC desc;
			pTex2d->GetDesc( &desc );
			if( srcRect.left > int( desc.Width ) || srcRect.left < 0 || srcRect.right > int( desc.Width ) || srcRect.right < 0 ||
				srcRect.top > int( desc.Height ) || srcRect.top < 0 || srcRect.bottom > int( desc.Height ) || srcRect.bottom < 0 )
			{
				const auto errorString = std::string( "Loading Sprite\nBad source rect\nRect: " ) +
					std::to_string( srcRect.top ) + "," + std::to_string( srcRect.bottom ) + "," + std::to_string( srcRect.left ) + "," + std::to_string( srcRect.right ) +
					std::string( "\nTex:\n" ) +
					std::to_string( desc.Width ) + "," + std::to_string( desc.Height );
				throw std::runtime_error( errorString.c_str() );
			}
		}

		origin = { float( srcRect.left + org.x ),float( srcRect.top + org.y ) };
	}		
	void Draw( DirectX::SpriteBatch& sb,const DirectX::XMFLOAT2& pos,float theta = 0.0f ) const
	{
		sb.Draw( pTex.Get(),pos,&srcRect,DirectX::Colors::White,theta,origin,scale );
	}
private:
	TextureCodex::TexturePtr pTex;
	DirectX::XMFLOAT2 origin;
	RECT srcRect;
	float scale;
};