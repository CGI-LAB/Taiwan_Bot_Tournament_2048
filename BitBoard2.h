#ifndef __BITBOARD2_H__
#define __BITBOARD2_H__

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
typedef unsigned long long myBoard_t;

class BitBoard
{
public:
	BitBoard(void);
	BitBoard(myBoard_t bitboard);
	~BitBoard(void);
	inline myBoard_t get_board() { return board_; }
	int count_empty_tile();
	void get_isomorphic_boards(BitBoard isomorphic_boards[]);

private:
	BitBoard get_flip_board();
	BitBoard get_mirror_board();
	BitBoard get_flip_diagonal_3_12_board();
	BitBoard get_flip_diagonal_0_15_board();

private:
	myBoard_t board_;
};

#endif
