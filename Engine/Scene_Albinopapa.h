#pragma once
#include "Game.h"
class Scene_Albinopapa :public Game::Scene
{
public:
	Scene_Albinopapa( Graphics &Gfx, MainWindow &Wnd );

	void UpdateModel( float DeltaTime )override;
	void ComposeFrame()override;

private:
	Graphics &gfx;
	MainWindow &wnd;



};

