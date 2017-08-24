/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "FrameTimer.h"
#include "Table.h"
#include "Sound.h"
#include <array>
#include <random>

enum class State
{
	Placing,
	Freefalling,
	Clearing,
	YousDed
};

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	void SpawnPiece();
	void SettlePiece();
	void UpdatePlacing();
	void UpdateFalling();
	void UpdateClearing();
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	FrameTimer timer;
	float t = 0.0f;
	float place_time = 0.4f;
	float fall_time = 0.08f;
	float clear_time = 0.55f;
	float clear_blink_time = 0.055f;
	int combo_level = 0;
	Table table = Table( 8,16,gfx );
	std::uniform_int_distribution<int> poo_color_dist = 
		std::uniform_int_distribution<int>{ 0,3 };
	std::mt19937 rng = std::mt19937( std::random_device{}() );
	Vec2 table_pos = { 
		(float( gfx.ScreenWidth ) - table.GetScreenWidth()) / 2.0f,
		(float( gfx.ScreenHeight ) - table.GetScreenHeight()) / 2.0f
	};
	Sound bad = L"Sounds\\bad.wav";
	Sound bump = L"Sounds\\bump.wav";
	Sound clear = L"Sounds\\clear.mp3";
	Sound rotate = L"Sounds\\rotate.wav";
	Sound sakura = Sound( L"Sounds\\sakura.mp3",Sound::LoopType::AutoFullSound );
	Sound settle = L"Sounds\\settle.wav";
	Piece p;
	State s;
	Table::PositionSet dying;
	Sprite bg = gfx.MakeSprite( L"images\\peewee.png",{ 0,0,296,552 },1.0f,{ 0.0f,0.0f } );
	/********************************/
};