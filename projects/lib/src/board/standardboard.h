/*
    This file is part of Cute Chess.
    Copyright (C) 2008-2018 Cute Chess authors

    Cute Chess is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cute Chess is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cute Chess.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STANDARDBOARD_H
#define STANDARDBOARD_H

#include "board.h"
#include "zobrist.h"
#include "result.h"
#include <QMutex>

namespace Chess {

/*! \brief Zobrist keys for Western chess variants */
class LIB_EXPORT WesternZobrist : public Zobrist
{
	public:
		WesternZobrist(const quint64* keys = nullptr);

		// Inherited from Zobrist
		virtual void initialize(int squareCount,
					int pieceTypeCount);
		virtual quint64 side() const;
		virtual quint64 piece(const Piece& piece, int square) const;

	private:
		int m_pieceIndex;
		QMutex m_mutex;
};

/*!
 * \brief A board for western chess variants
 *
 * WesternBoard serves as the overclass for all western variants.
 * In addition to possibly unique pieces, a western variant has
 * the same pieces as standard chess, the same rules for castling,
 * en-passant capture, promotion, etc.
 *
 * WesternBoard implements the rules of standard chess, including
 * check, checkmate, stalemate, promotion, 3-fold repetition,
 * 50 move rule and draws by insufficient material.
 */
class LIB_EXPORT WesternBoard : public Board
{
	public:
		/*! Basic piece types for western variants. */
		enum WesternPieceType
		{
			Pawn = 1,	//!< Pawn
			Xiang,		//!< Knight
			Shi,		//!< Bishop
			Pao,		//!< Rook
			Ma,		    //!< Ma
			Che,		//!< Queen
			King		//!< King
		};

		/*! Creates a new WesternBoard object. */
		WesternBoard(WesternZobrist* zobrist);

		// Inherited from Board
		virtual int width() const;
		virtual int height() const;
		virtual Result result();
		virtual int reversibleMoveCount() const;

	protected:
		/*! Types of Pawn moves. */
		enum StepType
		{
			 NoStep = 0,      //!< Cannot move here
			 FreeStep =  1,   //!< May move if target is empty
			 CaptureStep = 2  //!< Capture opposing piece only
		};

		/*!
		 * Returns true if both counts of kings given by
		 * \a whiteKings and \a blackKings are correct.
		 * WesternBoard expects exactly one king per side.
		 * \sa AntiBoard
		 * \sa HordeBoard
		 */
		virtual bool kingsCountAssertion(int whiteKings,
						 int blackKings) const;

		/*! Returns true if \a side is under attack at \a square. */
		virtual bool inCheck(Side side /*, int square = 0*/) const;

		/*!
		 * Returns FEN extensions. The default is an empty string.
		 */
		virtual QString vFenIncludeString(FenNotation notation) const;

		// Inherited from Board
		virtual void vInitialize();
		virtual QString vFenString(FenNotation notation) const;
		virtual bool vSetFenString(const QStringList& fen);
		virtual QString lanMoveString(const Move& move);
		virtual QString ChineseMoveString(const Move& move);
		virtual Move moveFromLanString(const QString& str);
		virtual void vMakeMove(const Move& move,
				       BoardTransition* transition);
		virtual void vUndoMove(const Move& move);
		virtual void generateMovesForPiece(QVarLengthArray<Move>& moves,
						   int pieceType,
						   int square) const;
		virtual bool vIsLegalMove(const Move& move);
		virtual bool isLegalPosition();
		virtual int captureType(const Move& move) const;

		virtual Move moveFromStringCN(const QString& str);

		/*! Returns the king square of \a side. */
		int kingSquare(Side side) const;

	private:
		// Data for reversing/unmaking a move
		struct MoveData
		{
			Piece capture;
			int reversibleMoveCount;
		};

		int m_arwidth;
		int m_sign;
		int m_kingSquare[2];
		int m_plyOffset;
		int m_reversibleMoveCount;

		QVector<MoveData> m_history;

		const WesternZobrist* m_zobrist;

		QVarLengthArray<int> m_BPawnOffsets;	    // 黑卒
		QVarLengthArray<int> m_RPawnOffsets;	    // 红兵
		QVarLengthArray<int> m_MaOffsets;
		QVarLengthArray<int> m_MaLegOffsets;             // 马腿
		QVarLengthArray<int> m_MaCheckLegOffsets;        // 别人的马将军的马腿

		QVarLengthArray<int> m_XiangOffsets;        // 相
		QVarLengthArray<int> m_XiangEyeOffsets;     // 象眼

		QVarLengthArray<int> m_CheOffsets;		// 车
		QVarLengthArray<int> m_ShiOffsets;		// 仕

		QVarLengthArray<QString> strnumCn;
		QVarLengthArray<QString> strnumEn;
		QVarLengthArray<QString> strnumName;

};

/*!
 * \brief A board for standard chess
 *
 * This is the most common chess variant, and one that is
 * supported by almost all chess engines.
 *
 * StandardBoard uses Polyglot-compatible zobrist position keys,
 * so Polyglot opening books can be used easily.
 *
 * \note Rules: http://www.fide.com/component/handbook/?id=124&view=article
 * \sa PolyglotBook
 */
class LIB_EXPORT StandardBoard : public WesternBoard
{
	public:
		/*! Creates a new StandardBoard object. */
		StandardBoard();

		// Inherited from WesternBoard
		virtual Board* copy() const;
		virtual QString variant() const;
		virtual QString defaultFenString() const;
		virtual Result tablebaseResult(unsigned int* dtm = nullptr) const;

};

} // namespace Chess
#endif // STANDARDBOARD_H
