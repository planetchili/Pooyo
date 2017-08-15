/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "ChiliMath.h"
#include <random>

namespace dx = DirectX;

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	std::mt19937 rng( std::random_device{}() );
	std::uniform_real_distribution<float> distX( 0.0f,float( gfx.ScreenWidth - 1 ) );
	std::uniform_real_distribution<float> distY( 0.0f,float( gfx.ScreenHeight - 1 ) );
	std::normal_distribution<float> distAnglularVelocity( PI,PI / 2.0f );

	for( size_t i = 0; i < positions.size(); i++ )
	{
		positions[i] = { distX( rng ),distY( rng ) };
		angularVelocities[i] = distAnglularVelocity( rng );
	}
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	t += timer.Mark();
}

void Game::ComposeFrame()
{
	auto batch = gfx.MakeSpriteBatch();

	batch.Begin( dx::SpriteSortMode_Deferred,
				 gfx.GetStates().NonPremultiplied(),
				 gfx.GetStates().PointClamp() );
	for( size_t i = 0; i < positions.size(); i++ )
	{
		const float angle = wrap_angle( t * angularVelocities[i] );
		marle.Draw( batch,positions[i],angle );	}

	batch.End();
}
