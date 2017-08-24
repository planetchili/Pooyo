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
	sakura.Play();
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
						bump.Play();
					}
					else
					{
						bad.Play();
					}
					break;
				case VK_RIGHT:
					if( !table.IsColliding( p.GetCopy().PushRight() ) )
					{
						p.PushRight();
						bump.Play();
					}
					else
					{
						bad.Play();
					}
					break;
				case 'Z':
					if( !table.IsColliding( --p.GetCopy() ) )
					{
						--p;
						rotate.Play();
					}
					else
					{
						bad.Play();
					}
					break;
				case 'X':
					if( !table.IsColliding( ++p.GetCopy() ) )
					{
						++p;
						rotate.Play();
					}
					else
					{
						bad.Play();
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
		UpdateClearing();
		break;
	case State::YousDed:
		break;
	}
}

/********************************/
/*  User Functions              */

void Game::SpawnPiece()
{
	p = Piece( { table.GetWidth() / 2,0 },
			   Puyo::Type( poo_color_dist( rng ) ),
			   Puyo::Type( poo_color_dist( rng ) ) );
	if( table.IsColliding( p ) )
	{
		s = State::YousDed;
	}
	combo_level = 0;
	s = State::Placing;
}

void Game::SettlePiece()
{
	//settle.Play();
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
			dying = table.FindDying();
			if( dying.size() > 0 )
			{
				t = 0.0f;
				s = State::Clearing;
				clear.Play( std::exp2( combo_level * 0.2f ) );
				combo_level++;
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
	if( t >= clear_time )
	{
		table.DestroyDying( dying );
		t = 0.0f;
		s = State::Freefalling;
	}
}

void Game::ComposeFrame()
{
	{
		auto sb = gfx.MakeSpriteBatch();
		sb.Begin( DirectX::SpriteSortMode_Deferred,
				  gfx.GetStates().NonPremultiplied() );
		bg.Draw( sb,table_pos - Vec2{40.0f,40.0f} );
		sb.End();
	}

	if( s == State::Placing )
	{
		p.Draw( gfx,table_pos );
	}

	if( s == State::Clearing )
	{
		if( int( t / clear_blink_time ) % 2 == 0 )
		{
			table.Draw( gfx,table_pos,dying );
		}
		else
		{
			table.Draw( gfx,table_pos );
		}
	}
	else
	{
		table.Draw( gfx,table_pos );
	}
}
