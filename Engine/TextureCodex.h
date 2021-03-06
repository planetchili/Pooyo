#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include <string>
#include <map>
#include "COMInitializer.h"

class TextureCodex
{
public:
	typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TexturePtr;
public:
	TextureCodex( Microsoft::WRL::ComPtr<ID3D11Device>& pDevice );
	// loads a texture from file or aquires texture if already loaded
	TexturePtr Acquire( const std::wstring& fileName );
	// destroy all textures that have no owners outside of codex
	void KillOrphans();
private:
	// count number of refs to the com object
	ULONG CountRefs( TexturePtr pTex ) const;
private:
	COMInitializer comInit;
	Microsoft::WRL::ComPtr<ID3D11Device>& pDevice;
	std::map<std::wstring,TexturePtr> texturePtrs;
};
