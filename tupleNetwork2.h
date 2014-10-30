#ifndef __TUPLENETWORK2_H__
#define __TUPLENETWORK2_H__

#include "BitBoard2.h"
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

struct BoardMoveRecord
{
	BitBoard after_move_board_;  // s'
	BitBoard after_add_tile_board_; // s"
};

class TupleNetwork
{
public:
	static TupleNetwork tuple_network;
	
public:
	void load_tuple_network(string filename);
	double get_board_value(BitBoard my_Board);
	
private:
	TupleNetwork();
	~TupleNetwork();
	void init_tuple_network();
	double get_row_tuple_1_value(myBoard_t my_Board);
	double get_row_tuple_2_value(myBoard_t my_Board);
	double get_square_tuple_1_value(myBoard_t my_Board);
	double get_square_tuple_2_value(myBoard_t my_Board);
	double get_big_tile_feature_value(myBoard_t my_Board);
	double get_empty_tile_feature_value(BitBoard my_Board);

private:
	double* row_tuple_1_;
	double* row_tuple_2_;
	double* square_tuple_1_;
	double* square_tuple_2_;
	double* big_tile_feature_type_;
	double* empty_tile_feature_;
};

#endif
