#ifndef _PRUNEEMM_H
#define _PRUNEEMM_H

#include "bitboard.h"
#include "zhash.h"
#include "tupleNetwork.h"
#include "tupleNetwork2.h"
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
//#include <omp.h>
//#define DEPTH_LIMIT 5
using namespace std;

//typedef pair<hashKey_t,double> boardInfo;
zHashTable transTable;
tupleNetwork tN[3];

int node = 0;
int AI = 1;
int RANDOM = 2;
double probability[2] = {0.9 , 0.1};
double currentScore = 0;
int iTupleSelect = 0;
bool isSmallerThan2048 = true;

double evaluate(board_t b){
		if(isSmallerThan2048){
			BitBoard chaochin(b);
			return TupleNetwork::tuple_network.get_board_value(chaochin);
		}else return tN[iTupleSelect].getValue(b);
};

class moveScore{
public:
	int move;
	board_t afterMove;
	double score;
	int moveReward;
};

bool moveScoreSort(moveScore m1, moveScore m2){
	return m1.score > m2.score;
};

double expectiminmax(board_t hashKey, double alpha, int player, int depth, bool& maybeDead){

	board checkBoard(hashKey);
	if((depth == 0)){
		node ++;
		return evaluate(hashKey);
	}else{


		if(player == AI){
			
			/*const scoreTable::iterator &it = transTable.find(hashKey);
			if(it != transTable.end()){
				return it->second;
			}*/
			
			//bool isMove = false;
			double tempAlpha = alpha;
			int bestMove = -1;
			
			if(transTable.exist(hashKey,depth))	return transTable.retrieve(hashKey,depth);
			
			// if there is already a best next move, move and return the result
			// else greedy choose the next best move by heuristic
			/*if(depth+2 <= DEPTH_LIMIT && transTable.exist(hashKey,depth+2))
			{
				bestMove = transTable.getBestMove(hashKey,depth+2);
				checkBoard.canMove(bestMove);
				return expectiminmax(checkBoard.getCurrentPosition(), tempAlpha, RANDOM, depth - 1);
			}else*/
			{		
				vector<moveScore> v;
				for(int i = 0; i < 4; i++){
					checkBoard.init(hashKey);
					int moveReward = 0;
					if((moveReward = checkBoard.canMove(i))!=-1){
						moveScore m;
						m.afterMove = checkBoard.getCurrentPosition();
						m.move = i;
						m.score = evaluate(checkBoard.getCurrentPosition());
						m.moveReward = moveReward;
						v.push_back(m);
					}
				}	

				// can not move
				if(v.size() == 0){
					node ++;
					maybeDead = true;
					return 0;/*checkBoard.eval();*/
				}

				//sort(v.begin(),v.end(),moveScoreSort);
				
				
				double tempAlphaForMaybeDead = alpha;
				int bestMoveForMaybeDead = -1;
				for(unsigned int i = 0; i < v.size(); i++){
					bool maybeDeadNextCall = false;
					double result = v[i].moveReward + expectiminmax(v[i].afterMove, tempAlpha, RANDOM, depth - 1, maybeDeadNextCall);
					
					if(result > tempAlpha && maybeDeadNextCall == false){
						tempAlpha = result;
						bestMove = v[i].move;
					}
					if(result > tempAlphaForMaybeDead){
						tempAlphaForMaybeDead = result;
						bestMoveForMaybeDead = v[i].move;
					}
				}
				if(bestMove == -1) {
					tempAlpha = tempAlphaForMaybeDead;
					bestMove = bestMoveForMaybeDead;
					maybeDead = true;
				}
			}
		
			//if(depth < 6) transTable[hashKey] = tempAlpha;	
			transTable.insert(hashKey,tempAlpha,depth,bestMove);	

			return tempAlpha;

		}else if(player == RANDOM){

			//double newAlpha = 0;
			double accumulatedAlpha = 0;
			int empty = 0;
			int sum  = rowSum[getRow(checkBoard.m_board,0)] + rowSum[getRow(checkBoard.m_board,1)] + rowSum[getRow(checkBoard.m_board,2)] + rowSum[getRow(checkBoard.m_board,3)];

			for(int i =0; i < 64; i+=4){
					int val = (checkBoard.m_board >> i) & 0xf;
					if(val == 0){
						empty++;
					}
			}

			double maxScore[2];
			//double minScore[2];
			for(int i = 2; i <= 4; i+=2){
				int finalSum = sum + i + (depth/2)*4;
				maxScore[i/2-1] = maxScoreOfSum[finalSum];
				finalSum = sum + i + (depth/2)*2;
				//minScore[i/2-1] = minScoreOfSum[finalSum];
			}

			double maxSumOfLeftChild = 0;
			maxSumOfLeftChild += probability[0]* maxScore[0] + probability[1]* maxScore[1];

			// can be parallelized for depth >= 5
			for(int i =0; i < 64; i+=4){
					if(((checkBoard.m_board >> i) & 0xf ) == 0){
						board_t mask = 0xf;
						mask = mask << i;
						mask = ~mask;
						for(int k = 1; k <= 2; k++){
							board_t val = k;
							maxSumOfLeftChild -= (probability[k-1] / empty) * maxScore[k-1];
							//newAlpha = (alpha - accumulatedAlpha - maxSumOfLeftChild) / (probability[k-1] /(double)empty);
							double tempAlpha = -1000000;// */newAlpha > minScore[k-1]? newAlpha : minScore[k-1];

							// since next node is max node again, impossible for alpha cut off to happen
							// still need to search all of its children
							bool maybeDeadNextCall = false;
							double result = expectiminmax((checkBoard.m_board & mask) | (val << i), tempAlpha, AI, depth - 1, maybeDeadNextCall);
							if(maybeDeadNextCall)
								maybeDead = true;
							//if(result <= newAlpha) return alpha;
							//if(result* probability[k-1] / empty <= alpha - accumulatedAlpha - maxSumOfLeftChild) return alpha;
							accumulatedAlpha += result* probability[k-1] / empty;
							

						}
					}		
			}
			
			return accumulatedAlpha;
		}

	}
	return 0;
}
#endif