#pragma once

#include "Graphics.h"
#include "MainWindow.h"
#include "Sprite.h"
#include <DirectXMath.h>
#include <array>

class Scene_Chili 
{
public:
	Scene_Chili( Graphics &Gfx, MainWindow &Wnd );

	void UpdateModel( float DeltaTime );
	void ComposeFrame();

private:
	Graphics &gfx;
	MainWindow &wnd;

	Sprite marle = gfx.MakeSprite( L"marle32x48.png", { 0, 0, 32, 48 }, 4.0f, { 16.0f, 24.0f } );
	float t = 0.0f;
	static constexpr size_t actorCount = 300u;
	std::array<DirectX::XMFLOAT2, actorCount> positions;
	std::array<float, actorCount> angularVelocities;
};

