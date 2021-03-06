#include "TextureCodex.h"
#include "Graphics.h"
#include "WICTextureLoader.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

TextureCodex::TextureCodex( wrl::ComPtr<ID3D11Device>& pDevice )
	:
	pDevice( pDevice )
{}

TextureCodex::TexturePtr TextureCodex::Acquire( const std::wstring& fileName )
{
	auto& pTex = texturePtrs[fileName];
	
	if( !pTex )
	{
		// if texture did not exist in map, new shared pointer owning empty comptr
		// will have been inserted and ppTex references it; we need to fill it
		HRESULT hr;
		if( FAILED( hr = dx::CreateWICTextureFromFile(
			pDevice.Get(),
			fileName.c_str(),
			nullptr,
			&pTex ) ) )
		{
			throw CHILI_GFX_EXCEPTION( hr,
				(L"Loading texture from bitmap.\nFilename: " + fileName).c_str() );
		}
	}
	return pTex;
}

void TextureCodex::KillOrphans()
{
	for( auto i = texturePtrs.begin(); i != texturePtrs.end(); )
	{
		if( CountRefs( i->second ) == 1u )
		{
			i = texturePtrs.erase( i );
		}
		else
		{
			++i;
		}
	}
}

// count number of refs to the com object
ULONG TextureCodex::CountRefs( TexturePtr pTex ) const
{
	pTex.Get()->AddRef();
	return pTex.Get()->Release();
}
