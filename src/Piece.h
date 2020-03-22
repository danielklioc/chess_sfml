class Piece
{
public:
	Piece(char colour) : pieceColor(colour) {}

	~Piece() {}

	virtual char GetPiece() = 0;

	char GetColor() {
		return pieceColor;
	}

	bool LegalMove(int sourceRow, int sourceColumn, int destinationRow, int destinationColumn)
	{
		return PieceLegalMove( sourceRow,  sourceColumn,  destinationRow,  destinationColumn);
	}

private:
	bool PieceLegalMove(int sourceRow, int sourceColumn, int destinationRow, int destinationColumn);
	char pieceColor;
};

class Pawn : public Piece
{
public:
	Pawn(char colour) : Piece(colour) {}
	~Pawn() {}
private:
	virtual char GetPiece()
	{
		return 'P';
	}

	bool PieceLegalMove(int sourceRow, int sourceColumn, int destinationRow, int destinationColumn)
	{
		return true;
	}
};