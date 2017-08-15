#pragma once
#include "Game.h"
class Scene_Spaceman :
	public Game::Scene
{
public:
	Scene_Spaceman( Graphics &Gfx, MainWindow &Wnd );

	void UpdateModel( float DeltaTime )override;
	void ComposeFrame()override;

private:
	Graphics &gfx;
	MainWindow &wnd;
};

