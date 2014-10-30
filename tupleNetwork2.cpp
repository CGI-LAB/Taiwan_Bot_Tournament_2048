#include "tupleNetwork2.h"

TupleNetwork TupleNetwork::tuple_network;

TupleNetwork::TupleNetwork()
{
	row_tuple_1_ = new double[65536];
	row_tuple_2_ = new double[65536];
	square_tuple_1_ = new double[16777216];
	square_tuple_2_ = new double[16777216];
	big_tile_feature_type_ = new double[1024];  // 32k ~ 2k, 0~3 tiles
	empty_tile_feature_ = new double[16];
	init_tuple_network();
}

TupleNetwork::~TupleNetwork()
{
	delete[] row_tuple_1_;
	delete[] row_tuple_2_;
	delete[] square_tuple_1_;
	delete[] square_tuple_2_;
	delete[] big_tile_feature_type_;
	delete[] empty_tile_feature_;
}

void TupleNetwork::init_tuple_network()
{
	memset(row_tuple_1_, 0, sizeof(double) * 65536);
	memset(row_tuple_2_, 0, sizeof(double) * 65536);
	memset(square_tuple_1_, 0, sizeof(double) * 16777216);
	memset(square_tuple_2_, 0, sizeof(double) * 16777216);
	memset(big_tile_feature_type_, 0, sizeof(double) * 1024);
	memset(empty_tile_feature_, 0, sizeof(double) * 16);
}

void TupleNetwork::load_tuple_network(string filename)
{
	ifstream fin(filename.c_str(), ios::in | ios::binary);
	if(!fin)
		return;
	fin.read((char*)row_tuple_1_, sizeof(double) * 65536);
	fin.read((char*)row_tuple_2_, sizeof(double) * 65536);
	fin.read((char*)square_tuple_1_, sizeof(double) * 16777216);
	fin.read((char*)square_tuple_2_, sizeof(double) * 16777216);
	fin.read((char*)big_tile_feature_type_, sizeof(double) * 1024);
	fin.read((char*)empty_tile_feature_, sizeof(double) * 16);
	fin.close();
}

double TupleNetwork::get_board_value(BitBoard my_Board)
{
	BitBoard isomorphic_boards[8];
	my_Board.get_isomorphic_boards(isomorphic_boards);
	double total_value = 0.0;
	for(int i = 0;i < 8;i++) {
		total_value += get_row_tuple_1_value(isomorphic_boards[i].get_board());
		total_value += get_row_tuple_2_value(isomorphic_boards[i].get_board());
		total_value += get_square_tuple_1_value(isomorphic_boards[i].get_board());
		total_value += get_square_tuple_2_value(isomorphic_boards[i].get_board());
	}
	total_value += get_big_tile_feature_value(my_Board.get_board());
	total_value += get_empty_tile_feature_value(my_Board);
	return total_value;
}

double TupleNetwork::get_row_tuple_1_value(myBoard_t my_Board)
{

	myBoard_t index = my_Board & 0xffff;
	return row_tuple_1_[index];
}

double TupleNetwork::get_row_tuple_2_value(myBoard_t my_Board)
{
	myBoard_t index = (my_Board & 0xffff0000) >> 16;
	return row_tuple_2_[index];
}

double TupleNetwork::get_square_tuple_1_value(myBoard_t my_Board)
{
	myBoard_t index = (my_Board & 0xfff0000) >> 4 | (my_Board & 0xfff);
	return square_tuple_1_[index];
}

double TupleNetwork::get_square_tuple_2_value(myBoard_t my_Board)
{
	myBoard_t index = (my_Board & 0xfff00000000) >> 20 | (my_Board & 0xfff0000) >> 16;
	return square_tuple_2_[index];
}

double TupleNetwork::get_big_tile_feature_value(myBoard_t my_Board)
{
	static int shift_pos[5] = {0, 2, 4, 6, 8};
	int index = 0;
	int count = 0;
	int tile = 0;
	for(myBoard_t mask = 0xf;mask != 0;mask <<= 4) {
		if((tile = (my_Board & mask) >> count) >= 11)
			index += 0x1ull << shift_pos[tile - 11];
		count += 4;
	}
	return big_tile_feature_type_[index];;
}

double TupleNetwork::get_empty_tile_feature_value(BitBoard my_Board)
{
	return empty_tile_feature_[my_Board.count_empty_tile()];
}
