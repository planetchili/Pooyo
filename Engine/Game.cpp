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
	SpawnPiece();
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
	switch( s )
	{
	case State::Placing:
		while( !wnd.kbd.KeyIsEmpty() )
		{
			const auto e = wnd.kbd.ReadKey();
			if( e.IsPress() )
			{
				switch( e.GetCode() )
				{
				case VK_LEFT:
					if( !table.IsColliding( p.GetCopy().PushLeft() ) )
					{
						p.PushLeft();
					}
					break;
				case VK_RIGHT:
					if( !table.IsColliding( p.GetCopy().PushRight() ) )
					{
						p.PushRight();
					}
					break;
				case 'Z':
					if( !table.IsColliding( --p.GetCopy() ) )
					{
						--p;
					}
					break;
				case 'X':
					if( !table.IsColliding( ++p.GetCopy() ) )
					{
						++p;
					}
					break;
				case VK_DOWN:
					if( s == State::Placing )
					{
						t = std::min( fall_time,t );
					}
					break;
				}
			}
		}
		UpdatePlacing();
		break;
	case State::Freefalling:
		UpdateFalling();
		break;
	case State::Clearing:
		break;
	case State::YousDed:
		break;
	}
}

/********************************/
/*  User Functions              */

void Game::SpawnPiece()
{
	p = Piece( { table.GetWidth() / 2,0 },Puyo::Type::Red,Puyo::Type::Blue );
	if( table.IsColliding( p ) )
	{
		s = State::YousDed;
	}
	s = State::Placing;
}

void Game::SettlePiece()
{
	table.LockPiece( p );
	s = State::Freefalling;
	t = fall_time;
	UpdateFalling();
}

void Game::UpdatePlacing()
{
	const float period = wnd.kbd.KeyIsPressed( VK_DOWN ) ?
		fall_time : place_time;
	if( t >= period )
	{
		if( table.IsResting( p ) )
		{
			SettlePiece();
		}
		else
		{
			p.Drop();
			t -= period;
		}
	}
}

void Game::UpdateFalling()
{
	if( t >= fall_time )
	{
		t -= fall_time;
		if( !table.DoFall() )
		{
			const auto dying = table.FindDying();
			if( dying.size() > 0 )
			{
				table.DestroyDying( dying );
			}
			else
			{
				t = 0.0f;
				SpawnPiece();
			}
		}
	}
}

void Game::UpdateClearing()
{

}

void Game::ComposeFrame()
{
	if( s == State::Placing )
	{
		p.Draw( gfx,table_pos );
	}
	table.Draw( gfx,table_pos );
}
