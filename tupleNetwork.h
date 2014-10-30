#ifndef _TUPLENETWORK_H
#define _TUPLENETWORK_H

#include "bitboard.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class tupleNetwork{
public:
	tupleNetwork(){
		fourTuple = new double*[2];
		sixTuple = new double*[2];
		maxTileFeature = new double[1024];
		for(int i = 0; i < 2; i++){
			fourTuple[i] = new double[0xffff+1];
			sixTuple[i] = new double[0xffffff+1];
			//memset(fourTuple[i],0,sizeof(double)*(0xffff+1));
			//memset(sixTuple[i],0,sizeof(double)*(0xffffff+1));
		}
	}
	~tupleNetwork(){
		for(int i = 0; i < 2; i++){
			delete[] fourTuple[i];
			delete[] sixTuple[i];
		}
		delete[] fourTuple;
		delete[] sixTuple;
		delete[] maxTileFeature;
	};
	unsigned long long getFourTupleIndex(board_t b, int num){
		if(num!=0 && num!=1) while(1);
		row_t r = getCol(b,3-num);
		return r;
	};
	unsigned long long getSixTupleIndex(board_t b, int num){
		if(num!=0 && num!=1) while(1);
		
		board_t r1 = getCol(b,2-num);
		board_t r2 = reverseRow(getCol(b,1-num));
		board_t finalIndex = (r1&0xfff0) << 8 | (r2&0xfff);
		
		return finalIndex;
	};
	unsigned long long getMaxFeatureIndex(board_t b){
		unsigned long long index = 0;
		for(int i = 0; i < 64; i+= 4){
			int val = (b >> i)&0xf;
			int shift = val - 11;
			if(val >= 11) index += val << shift;
		}
		return index;
	};
	double getValue(board_t boardStatus){
		board_t b[8];
		row_t rows[4];
		row_t reverseRows[4];
		row_t oRows[4];
		row_t oReverseRows[4];
	
		for(int i =0; i < 4; i++){
			rows[i] = getRow(boardStatus,i);
			oRows[3-i] = rows[i];
			reverseRows[i] = reverseRow(rows[i]);
			oReverseRows[3-i] = reverseRows[i];
		}
	
	
		b[0] = boardStatus;
		b[1] = setRows(reverseRows);
		b[2] = setRows(oRows);
		b[3] = setRows(oReverseRows);
		b[4] = setCols(rows);
		b[5] = setCols(reverseRows);
		b[6] = setCols(oRows);
		b[7] = setCols(oReverseRows);

		double sum[8] = {0.0};
		double totalSum = 0.0;
		for(int j = 0; j < 8; j++){
			for(int i = 0; i < 2; i++){
				sum[j] += fourTuple[i][getFourTupleIndex(b[j],i)];
				sum[j] += sixTuple[i][getSixTupleIndex(b[j],i)];
			}
			//if(sum[j] > maxSum) maxSum = sum[j];
			totalSum  += sum[j];
		}
		totalSum += maxTileFeature[getMaxFeatureIndex(b[0])];
		return  totalSum;
	};
	void updateValue(board_t boardStatus, double delta){
		board_t b[8];
		row_t rows[4];
		row_t reverseRows[4];
		row_t oRows[4];
		row_t oReverseRows[4];
	
		for(int i =0; i < 4; i++){
			rows[i] = getRow(boardStatus,i);
			oRows[3-i] = rows[i];
			reverseRows[i] = reverseRow(rows[i]);
			oReverseRows[3-i] = reverseRows[i];
		}
	
	
		b[0] = boardStatus;
		b[1] = setRows(reverseRows);
		b[2] = setRows(oRows);
		b[3] = setRows(oReverseRows);
		b[4] = setCols(rows);
		b[5] = setCols(reverseRows);
		b[6] = setCols(oRows);
		b[7] = setCols(oReverseRows);

		for(int j = 0; j < 8; j++){
			for(int i = 0; i < 2; i++){
				fourTuple[i][getFourTupleIndex(b[j],i)] += delta;
				sixTuple[i][getSixTupleIndex(b[j],i)] += delta;
			}
		}
		maxTileFeature[getMaxFeatureIndex(b[0])] += delta;
	};
	void saveResultToFile(int mode){
		fstream out;
		
		if(mode == 0)out.open("trainingResult(Binary)_before8192.txt", ios::out | ios::binary);
		if(mode == 1)out.open("trainingResult(Binary)_8192Appear.txt", ios::out | ios::binary);
		if(mode == 2)out.open("trainingResult(Binary)_16384_8192Appear.txt", ios::out | ios::binary);
		
		for(int i = 0; i < 2; i++){
			out.write(reinterpret_cast<char*>(fourTuple[i]), 65536*sizeof(double));
		}
		
		for(int i = 0; i < 2; i++){
			out.write(reinterpret_cast<char*>(sixTuple[i]), 0x1000000*sizeof(double));
		}
		out.write(reinterpret_cast<char*>(maxTileFeature), 1024*sizeof(double));
	
		/*out.open("trainingResult(BestTuple).txt", ios::out | ios::binary);
		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 65536; j++){
				out << fourTuple[i][j] << endl;
			}
		}

		for(int i = 0; i < 2; i++){
			for(board_t j = 0; j <= 0xffffff; j++){
				out << sixTuple[i][j] << endl;
			}
		}	
		*/
	};

	void getResultFromFile(int mode){
	
		fstream in;
			
		if(mode == 0)in.open("trainingResult(Binary)_before8192.txt", ios::in | ios::binary);
		if(mode == 1)in.open("trainingResult(Binary)_8192Appear.txt", ios::in | ios::binary);
		if(mode == 2)in.open("trainingResult(Binary)_16384_8192Appear.txt", ios::in | ios::binary);
		
		for(int i = 0; i < 2; i++){
			in.read(reinterpret_cast<char*>(fourTuple[i]), 65536 * sizeof(double));
		}
		for(int i = 0; i < 2; i++){
			in.read(reinterpret_cast<char*>(sixTuple[i]), 0x1000000 * sizeof(double));
		}
		in.read(reinterpret_cast<char*>(maxTileFeature), 1024*sizeof(double));
	
		/*
		in.open("trainingResult(BestTuple).txt", ios::in | ios::binary);
		for(int i = 0; i < 2; i++){
			for(int j = 0; j < 65536; j++){
				string value;
				in >> value;
				fourTuple[i][j] = atof(value.c_str());
			}
		}

		for(int i = 0; i < 2; i++){
			for(board_t j = 0; j <= 0xffffff; j++){
				string value;
				in >> value;
				sixTuple[i][j] = atof(value.c_str());
			}
		}	*/
	};
private:
	double** fourTuple;
	double** sixTuple;
	double* maxTileFeature;
};
#endif