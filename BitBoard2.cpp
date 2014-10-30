#include "BitBoard2.h"

BitBoard::BitBoard(void):
board_(0)
{
}

BitBoard::BitBoard( myBoard_t bitboard ):
board_(bitboard)
{
}

BitBoard::~BitBoard(void)
{
}

int BitBoard::count_empty_tile()
{
	int count = 0;
	myBoard_t tile_mask = 0xf;
	for(;tile_mask & 0xffffffffffffffff;tile_mask <<= 4) {
		if((board_ & tile_mask) == 0)
			count++;
	}
	return count;
}

void BitBoard::get_isomorphic_boards(BitBoard isomorphic_boards[])
{
	BitBoard flip_board = get_flip_board();
	BitBoard mirror_board = get_mirror_board();
	BitBoard flip_diagonal_3_12_board = get_flip_diagonal_3_12_board();
	BitBoard flip_diagonal_0_15_board = get_flip_diagonal_0_15_board();
	isomorphic_boards[0] = *this;
	isomorphic_boards[1] = flip_board;
	isomorphic_boards[2] = mirror_board;
	isomorphic_boards[3] = flip_board.get_mirror_board();
	isomorphic_boards[4] = flip_diagonal_3_12_board;
	isomorphic_boards[5] = flip_diagonal_0_15_board;
	isomorphic_boards[6] = flip_diagonal_3_12_board.get_flip_board();
	isomorphic_boards[7] = flip_diagonal_0_15_board.get_flip_board();
}

BitBoard BitBoard::get_flip_board()
{
  	return (board_ & 0xffff000000000000) >> 48 |
           (board_ & 0x0000ffff00000000) >> 16 |
           (board_ & 0x00000000ffff0000) << 16 |
           (board_ & 0x000000000000ffff) << 48;
}

BitBoard BitBoard::get_mirror_board()
{
  	return (board_ & 0xf000f000f000f000) >> 12 |
           (board_ & 0x0f000f000f000f00) >> 4 |
           (board_ & 0x00f000f000f000f0) << 4 |
           (board_ & 0x000f000f000f000f) << 12;
}

BitBoard BitBoard::get_flip_diagonal_3_12_board()
{
  	return (board_ & 0x000f00f00f00f000) |
           (board_ & 0x00f00f00f0000000) >> 20 |
           (board_ & 0x0000000f00f00f00) << 20 |
           (board_ & 0x0f00f00000000000) >> 40 |
           (board_ & 0x00000000000f00f0) << 40 |
           (board_ & 0xf000000000000000) >> 60 |
           (board_ & 0x000000000000000f) << 60;
}

BitBoard BitBoard::get_flip_diagonal_0_15_board()
{
  	return (board_ & 0xf0000f0000f0000f) |
           (board_ & 0x0f0000f0000f0000) >> 12 |
           (board_ & 0x0000f0000f0000f0) << 12 |
           (board_ & 0x00f0000f00000000) >> 24 |
           (board_ & 0x00000000f0000f00) << 24 |
           (board_ & 0x000f000000000000) >> 36 |
           (board_ & 0x000000000000f000) << 36;
}
