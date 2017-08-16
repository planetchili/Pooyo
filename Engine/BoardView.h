#pragma once

#include "Graphics.h"
#include <list>

class BoardView
{
public:
	BoardView( BoardModel &Model );

private:
	static constexpr int m_columns = 6;
	static constexpr int m_rows = 12;
	static constexpr int m_tileSize = Graphics::ScreenHeight / m_rows;
	
	static constexpr int m_horizPadding = 100;
	
	static constexpr int m_leftWinX = m_horizPadding;
	static constexpr int m_rightWinX = Graphics::ScreenWidth - ( ( m_columns * m_tileSize ) + m_horizPadding );

	static constexpr int m_numTiles = m_columns * m_rows;
	
	class BoardModel &m_model;
};

