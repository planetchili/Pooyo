#pragma once

#include "Piece.h"
#include <memory>
#include <vector>

class Board
{
public:
	class Cell
	{
	public:
		void RegisterPooYo( Color C );
		Color UnregisterPooYo();
		bool IsEmpty()const;
		void MakeEmpty();
		bool IsReserved()const;
		void SetReserved();
		void ClearReserved();
	public:
		Color color = Colors::Black;
		bool reserved = false;
	};
public:
	Board() = default;
	Board( float PosX, float PosY, int NumColumns, int NumRows );

	bool CanSpawn()const;
	Vec2f GetSpawnPosUpper()const;
	Vec2f GetSpawnPosLower()const;

	bool CanMoveLeft( const Vec2f &Pos0, const Vec2f &Pos1 )const;
	bool CanMoveRight( const Vec2f &Pos0, const Vec2f &Pos1 )const;
	bool CanRotate( const Vec2f &Pos0, const Vec2f &Pos1 )const;

	Vec2f ToLeft( const Vec2f &Pos )const;
	Vec2f ToUpper( const Vec2f &Pos )const;
	Vec2f ToRight( const Vec2f &Pos )const;
	Vec2f ToLower( const Vec2f &Pos )const;

	void CheckForColorChains( const Vec2f Pos );
	void AcceptNuisancePooYo( int Count );

	void RemoveChains();

	Vec2f LastAvailableCell( const Vec2f &Pos )const;
	bool IsInLastAvailableCell( const Vec2f &Pos )const;

	void ReserveCell( const Vec2f &Pos );
	void HandleCellRegistry( const Rectf &PooYoRect );

private:
	Vec2i ToGrid( const Vec2f &Pos )const;
	Vec2f ToPixel( const Vec2i &Pos )const;
	int MakeIndex( const Vec2i &GridPos )const;

public:
	static constexpr int cellSize = 40;
	static constexpr float fCellSize = static_cast< float >( cellSize );

	Vec2f boardPos;
	int numCols, numRows;
	int colorChainCombo;
	std::vector<Cell> cells;
	std::vector<int> colorchainIndices, nuisancesIndices;

	bool pieceIsMoving = false;
};

