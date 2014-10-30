#include "2048.h"
#include <math.h>
#include <time.h>
#include "bitboard.h"
#include "pruneEmm.h"
#include "tupleNetwork2.h"
#include <iostream>
using namespace std;



board_t transformGridToBoard(Grid grid){
	board_t b = 0;
	for(int i = 0; i < 16; i++){
		b = b << 4;
		int val = grid[i];
		if(val != 0) b = b | log2val[val];
	}
	return b;
};

void init(){
	init_ratio_table();
	init_tile_score();
	init_ct_score();
	init_score_table();
	init_right_info();
	init_MinMaxSum();
	init_rowSum();
	init_log2();
	tN[0].getResultFromFile(0);
	tN[1].getResultFromFile(1);
	tN[2].getResultFromFile(2);
	TupleNetwork::tuple_network.load_tuple_network("tuple_network_it_8192.data");
};

dir_e findBestMove(board_t b){
	board initialBoard;
	initialBoard.init(b);
	int bestMove = -1;
	
	//clock_t startTime = clock();
	int startDepth = 2;
	int endDepth = 4;
	if(iTupleSelect == 2) endDepth += 2;
	// iterative deepening
	while(startDepth <= endDepth){
	
		if(transTable.exist(b,startDepth+1))
		{
			bestMove = transTable.getBestMove(b,startDepth+1);
		}
		else
		{
			vector<moveScore> v;
			for(int i = 0; i < 4; i++){
				initialBoard.init(b);
				int moveReward = 0;
					if((moveReward = initialBoard.canMove(i))!=-1){
						moveScore m;
						m.afterMove = initialBoard.getCurrentPosition();
						m.move = i;
						m.score = evaluate(initialBoard.getCurrentPosition());
						m.moveReward = moveReward;
						v.push_back(m);
					}
			}
			//sort(v.begin(),v.end(),moveScoreSort);
			double alpha = -1000000;
					double deadAlpha = -1000000;
					int deadBestMove = -1;
					for(unsigned int i = 0; i < v.size(); i++){
						bool maybeDead = false;
						double result = v[i].moveReward + expectiminmax(v[i].afterMove, alpha, RANDOM, startDepth, maybeDead);
						if(alpha < result && maybeDead == false){
							alpha = result;
							bestMove = v[i].move;
						}
						if(deadAlpha < result) {
							deadAlpha = result;
							deadBestMove = v[i].move;
						}
					}
					if(bestMove == -1) {
						alpha = deadAlpha;
						bestMove = deadBestMove;
					}
					transTable.insert(b,alpha,startDepth+1,bestMove);
				}
				startDepth += 2;
	}
	
	dir_e dir = LEFT;
	switch(bestMove){
		case 0:
			dir = UP;
			break;
		case 1:
			dir = RIGHT;
			break;
		case 2:
			dir = DOWN;
			break;
		case 3:
			dir = LEFT;
			break;
		default:
			dir = INVALID;
			break;
	}
	
	return dir;
};

void PlayNRounds(int n){

    Game myGame;
    bool isGameOver;
    dir_e dir;
	
    for(int i = 0;i < n;i++){    
		//cout << "Playing Game: " << i;
        isGameOver = false;
		iTupleSelect = 0;
		isSmallerThan2048 = true;
        while(!isGameOver){
		
			Grid currentGrid;
			myGame.getCurrentGrid(currentGrid);
			board_t currentBoard = transformGridToBoard(currentGrid);
			board_t tempKey = currentBoard;
			//int maxTile = 0;
			bool is8192Appear = false;
			bool is16384Appear = false;
			for(int j = 0; j < 16; j++){
				int val = tempKey&0xf;
				if(val == 13) is8192Appear = true;
				if(val == 14) is16384Appear = true;
				if(val == 11) isSmallerThan2048 = false;
				
				tempKey  = tempKey  >> 4;
			}
			
			// checking if >= 8192 tile exist
			if(is8192Appear && is16384Appear) iTupleSelect = 2;
			else if(is8192Appear) iTupleSelect = 1;
			
			dir = findBestMove(currentBoard);
               
            myGame.insertDirection(dir);
            isGameOver = myGame.isGameOver();

        }
		//cout << "\tScore : " << myGame.getScore() << endl;
        //myGame.printGrid(35,2);
        if(i < n - 1)  myGame.reset();
    }
}

int main(int argc, char* argv[]){
    srand(time(NULL));
	init();
	PlayNRounds(100);
	
    return 0;
}
