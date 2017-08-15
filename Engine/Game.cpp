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
#include "Scene_Chili.h"
#include "Scene_Albinopapa.h"
#include "Scene_Spaceman.h"



Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	Choose( eChili );
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
	Choose( MakeChoice() );
	m_pScene->UpdateModel( timer.Mark() );
}

int Game::MakeChoice()
{
	const auto key = wnd.kbd.ReadKey();

	if( key.IsPress() )
	{
		const char upperLimit = static_cast< char >( Developer::eLast ) + '0';
		if( key.GetCode() >= '0' && key.GetCode() <= upperLimit )
		{
			return static_cast< int >( key.GetCode() - '0' );
		}		
	}

	return m_sceneIdx;
}

void Game::Choose( int Choice )
{
	if( m_sceneIdx == Choice ) return;

	m_sceneIdx = Choice;
	const auto choice = static_cast< Developer >( Choice );
	switch( choice )
	{
		case Developer::eSpaceman:
			m_pScene = std::make_unique<Scene_Spaceman>( gfx, wnd );
			break;
		case Developer::eChili:
			m_pScene = std::make_unique<Scene_Chili>( gfx, wnd );
			break;
		case Developer::eAlbinopapa:
			m_pScene = std::make_unique<Scene_Albinopapa>( gfx, wnd );
			break;
		default:
			throw std::runtime_error( std::string( __FUNCTION__ ) + "Out of range" );
			break;
	}
}

void Game::ComposeFrame()
{
	m_pScene->ComposeFrame();	
}
