class Piece
{
public:
	Piece(char cColor) : mcColor(cColor) {}

	~Piece() {}

	virtual char GetPiece() = 0;

	char GetColor() {
		return mcColor;
	}

	bool IsLegalMove(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, Piece* qpaaBoard[8][8])
	{
		Piece* qpDest = qpaaBoard[iDestRow][iDestCol];
		if ((qpDest == 0) || (mcColor != qpDest->GetColor()))
		{
			return AreSquaresLegal(iSrcRow, iSrcCol, iDestRow, iDestCol, qpaaBoard);
		}
		return false;
	}

private:
	virtual bool AreSquaresLegal(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, Piece* qpaaBoard[8][8]) = 0;
	char mcColor;
};

class Pawn : public Piece
{
public:
	Pawn(char cColor) : Piece(cColor) {}
	~Pawn() {}
private:
	virtual char GetPiece()
	{
		return 'P';
	}

	bool AreSquaresLegal(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, Piece* qpaaBoard[8][8]) {
		Piece* qpDest = qpaaBoard[iDestRow][iDestCol];
		if (qpDest == 0) {
			// Destination square is unoccupied
			if (iSrcCol == iDestCol) {
				if (GetColor() == 'W') {
					if (iDestRow == iSrcRow + 1) {
						return true;
					}
				}
				else {
					if (iDestRow == iSrcRow - 1) {
						return true;
					}
				}
			}
		}
		else {
			// Dest holds piece of opposite color
			if ((iSrcCol == iDestCol + 1) || (iSrcCol == iDestCol - 1)) {
				if (GetColor() == 'W') {
					if (iDestRow == iSrcRow + 1) {
						return true;
					}
				}
				else {
					if (iDestRow == iSrcRow - 1) {
						return true;
					}
				}
			}
		}
		return false;
	}
};
