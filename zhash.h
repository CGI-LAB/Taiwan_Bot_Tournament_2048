#ifndef _ZHASH_H
#define	_ZHASH_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "bitboard.h"
using namespace std;

typedef unsigned long long u8;
typedef struct ranctx { u8 a; u8 b; u8 c; u8 d; } ranctx;

#define rot(x,k) (((x)<<(k))|((x)>>(64-(k))))
u8 ranval( ranctx *x ) {
    u8 e = x->a - rot(x->b, 7);
    x->a = x->b ^ rot(x->c, 13);
    x->b = x->c + rot(x->d, 37);
    x->c = x->d + e;
    x->d = e + x->a;
    return x->d;
}
 
void raninit( ranctx *x, u8 seed ) {
    u8 i;
    x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
    for (i=0; i<20; ++i) {
        (void)ranval(x);
    }
}

#define zHashSeed 0

class kvPair{
public:
	u8 key;
	double value;
	short remainDepth;
	short bestMove;
};

class zHashTable{
public:
	zHashTable(){
		m_seed = zHashSeed;
		raninit(&m_rand,m_seed);
		mask = 0x3ffff;
		zHashTableSize	= sizeof(kvPair)*(mask+1);
		m_table = new kvPair[mask+1];
		memset(m_table, 0, zHashTableSize);

		for(int i = 0; i < 16; i++){
			for(int j = 0; j < 16; j++){
				m_hashTable[i][j] = ranval(&m_rand);
			}
		}
		for(int i = 0; i < 20; i++){
			m_depthHashTable[i] = ranval(&m_rand);
		}
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 65536; j++){
				m_fastHashTable[j+65536*i] = 	m_hashTable[(j>>12)&0xf][4*i]
											^	m_hashTable[(j>>8)&0xf][4*i+1]
											^	m_hashTable[(j>>4)&0xf][4*i+2]
											^	m_hashTable[j&0xf][4*i+3];
			}
		}
	}
	~zHashTable(){delete[] m_table;}
	void clear(){
		memset(m_table, 0, zHashTableSize);
	}
	u8 getIndex(board_t boardStatus, int depth){
		u8 index = 		m_fastHashTable[getRow(boardStatus,0)] 
					^ 	m_fastHashTable[getRow(boardStatus,1)+65536]
					^	m_fastHashTable[getRow(boardStatus,2)+131072]
					^	m_fastHashTable[getRow(boardStatus,3)+196608]
					^	m_depthHashTable[depth];
		return index;
	}
	double retrieve(board_t boardStatus, int depth){
		u8 index =	getIndex(boardStatus,depth);
		return m_table[index&mask].value;
	}
	short getBestMove(board_t boardStatus, int depth){
		u8 index =	getIndex(boardStatus,depth);
		return m_table[index&mask].bestMove;
	}
	bool exist(board_t boardStatus, int depth){
		u8 index =	getIndex(boardStatus,depth);
		if(m_table[index&mask].key == index && m_table[index&mask].remainDepth == depth){
			return true;
		}else return false;
	}
	void insert(board_t boardStatus, double score, int depth, int bestMove){
		u8 index =	getIndex(boardStatus,depth);
		m_table[index&mask].key = index;
		m_table[index&mask].value = score;
		m_table[index&mask].remainDepth = (short)depth;
		m_table[index&mask].bestMove = (short)bestMove;
	}
	
private:
	ranctx m_rand;
	u8	m_seed;
	u8	m_hashTable[16][16];
	u8	m_fastHashTable[65536*4];
	u8	m_depthHashTable[20];
	u8	mask;
	kvPair* m_table;
	int zHashTableSize;
};



#endif
