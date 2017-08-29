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
#include "Board.h"

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
					if( !p.Clone().PushLeft().IsCollidingIn( board ) )
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
					if( !p.Clone().PushRight().IsCollidingIn( board ) )
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
					if( !p.Clone().CCWRotate().IsCollidingIn( board ) )
					{
						p.CCWRotate();
						rotate.Play();
					}
					else
					{
						bad.Play();
					}
					break;
				case 'X':
					if( !p.Clone().CWRotate().IsCollidingIn( board ) )
					{
						p.CWRotate();
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
						// if we don't remove excess t here
						// could get multiple fast drops in
						// a single frame!
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
	p = Piece( { board.GetWidth() / 2,0 },
			   Puyo::Type( poo_color_dist( rng ) ),
			   Puyo::Type( poo_color_dist( rng ) ) );
	if( p.IsCollidingIn( board ) )
	{
		s = State::YousDed;
		return;
	}
	combo_level = 0;
	s = State::Placing;
}

void Game::SettlePiece()
{
	//settle.Play();
	p.LockInto( board );
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
		if( p.IsRestingIn( board ) )
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
		if( !board.DoFall() )
		{
			dying = board.FindDying();
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
		board.DestroyDying( dying );
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
		bg.Draw( sb,board_pos - Vec2{35.0f,35.0f} );
		sb.End();
	}

	if( s == State::Placing )
	{
		p.Draw( gfx,board_pos );
	}

	if( s == State::Clearing )
	{
		if( int( t / clear_blink_time ) % 2 == 0 )
		{
			board.Draw( gfx,board_pos,dying );
		}
		else
		{
			board.Draw( gfx,board_pos );
		}
	}
	else if( s != State::YousDed )
	{
		board.Draw( gfx,board_pos );
	}
}
