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


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	board( Graphics::ScreenWidth / 3, 0, 6, 12 ),
	red( gfx.MakeSprite( L"Images/red.png", { 0, 0, 32, 32 }, 1.f, { 15.f, 15.f } ) ),
	green( gfx.MakeSprite( L"Images/green.png", { 0, 0, 32, 32 }, 1.f, { 15.f, 15.f } ) ),
	yellow( gfx.MakeSprite( L"Images/yellow.png", { 0, 0, 32, 32 }, 1.f, { 15.f, 15.f } ) ),
	blue( gfx.MakeSprite( L"Images/blue.png", { 0, 0, 32, 32 }, 1.f, { 15.f, 15.f } ) ),
	gray( gfx.MakeSprite( L"Images/black.png", { 0, 0, 32, 32 }, 1.f, { 15.f, 15.f } ) )
{
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
	const float dt = std::min( timer.Mark(), .033f );

	switch( gamestate )
	{
		case State::Spawn:
			if( board.CanSpawn() )
			{
				std::random_device rd;
				std::mt19937 rng( rd() );
				std::uniform_int_distribution<int> clrDist( 0, 3 );
				const Color colors[]
				{ 
					Colors::Red, 
					Colors::Green, 
					Colors::Yellow, 
					Colors::Blue
				};

				currentPair = 
					Piece( board, colors[ clrDist( rng ) ], colors[ clrDist( rng ) ] );
				gamestate = State::Fall;
			}
			else
			{
				gamestate = State::GameOver;
			}
			break;
		case State::Fall:
			DoInput();
			if( !currentPair.BothHaveSettled() )
			{
				currentPair.Update( dt );
				board.HandleCellRegistry(
					currentPair.pPooyos[ 0 ].prevPosition,
					currentPair.pPooyos[ 0 ].position );
				board.HandleCellRegistry(
					currentPair.pPooyos[ 1 ].prevPosition,
					currentPair.pPooyos[ 1 ].position );
			}
			else
			{
				auto pooPair = currentPair.Collect();
				board.ClearReservation( pooPair.first.position );
				board.ClearReservation( pooPair.second.position );
				pooyos.push_back( std::move( pooPair.first ) );
				pooyos.push_back( std::move( pooPair.second ) );

				gamestate = State::ChainSearch;
			}
			break;
		case State::ChainSearch:
		{
			const size_t primIdx = pooyos.size() - 1u;
			const size_t scndIdx = pooyos.size() - 2u;

			board.CheckForColorChains( pooyos[ primIdx ].position );
			board.CheckForColorChains( pooyos[ scndIdx ].position );

			if( board.IsInLastAvailableCell( pooyos[ primIdx ].position ) ||
				board.IsInLastAvailableCell( pooyos[ scndIdx ].position ) )
			{
				gamestate = State::Spawn;
			}
			else 
			{
				gamestate = State::FillHoles;
			}
			break;
		}
		case State::FillHoles:
		{
			bool allSettled = false;
			for( auto &pooyo : pooyos )
			{
				if( !board.IsInLastAvailableCell( pooyo.position ) )
				{
					pooyo.Update( dt );
				}
				else
				{
					allSettled &= true;
				}
			}

			if( allSettled )
			{
				gamestate = State::ChainSearch;
			}
			break;
		}
	}
}

void Game::DoInput()
{
	if( wnd.kbd.KeyIsPressed( VK_LEFT ) )
	{
		currentPair.MoveLeft( board );
	}
	else if( wnd.kbd.KeyIsPressed( VK_RIGHT ) )
	{
		currentPair.MoveRight( board );
	}
	else if( wnd.kbd.KeyIsPressed( VK_SPACE ) )
	{
		currentPair.RotateCW( board );
	}
}

void Game::ComposeFrame()
{
	auto sb = gfx.MakeSpriteBatch();
	sb.Begin();
	auto SetSprite = [ this ](Color color)
	{
		if( color == Colors::Red )
		{
			return &red;
		}
		else if( color == Colors::Green )
		{
			return &green;
		}
		else if( color == Colors::Yellow )
		{
			return &yellow;
		}
		else if( color == Colors::Blue )
		{
			return &blue;
		}
		else if( color == Colors::Gray )
		{
			return &gray;
		}
	};

	if( gamestate != State::FillHoles )
	{
		{
			const Sprite *pSprite = SetSprite( currentPair.pPooyos[ 0 ].color );
			pSprite->Draw( sb, currentPair.pPooyos[ 0 ].position );
		}
		{
			const Sprite *pSprite = SetSprite( currentPair.pPooyos[ 1 ].color );
			pSprite->Draw( sb, currentPair.pPooyos[ 1 ].position );
		}
	}
	for( const auto &pooyo : pooyos )
	{
		const Sprite *pSprite = SetSprite( pooyo.color );		
		pSprite->Draw( sb, pooyo.position );
	}

	sb.End();
}
