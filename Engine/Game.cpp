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
#include "Puyo.h"
#include "Table.h"

namespace dx = DirectX;

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	table.At( { 0,0 } ) = Puyo( Puyo::Type::Red );
	table.At( { 2,1 } ) = Puyo( Puyo::Type::Red );
	table.At( { 2,1 } ) = Puyo( Puyo::Type::Red );
	table.At( { 11,19 } ) = Puyo( Puyo::Type::Green );
	table.At( { 11,18 } ) = Puyo( Puyo::Type::Blue );
	table.At( { 10,19 } ) = Puyo( Puyo::Type::Jama );
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
	table.Draw( gfx,{ 200.0f,20.0f } );
}
