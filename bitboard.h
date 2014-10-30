#ifndef _BITBOARD_H
#define _BITBOARD_H

#include <iostream>
#include <stdlib.h> 
#include <stdint.h>
#include <time.h>


#define COMMON_RATIO 0.25
using namespace std;

typedef uint16_t row_t;
typedef uint64_t board_t;

double COMMON_RATIOS[16];
int tile_score[16];
double ct_score[16*16];
double line_score[65536][4];
double maxScoreOfSum[65536];
double minScoreOfSum[65536];
int rowSum[0xffff];
int log2val[65536];


void init_ratio_table();
void init_tile_score();
void init_ct_score();
void init_score_table();
double getScore(board_t b);
void init_right_info();
void init_MinMaxSum();
void init_rowSum();
void init_log2();
row_t getRow(board_t b,int r);
row_t getCol(board_t b,int r);
board_t setRow(board_t b,int r, row_t row);
board_t setCol(board_t b,int c, row_t col);
board_t setRows(row_t* rows);
board_t setCols(row_t* cols);
row_t reverseRow(row_t r);
int moveRight(row_t& r);

class moveRightInfo{
public:
	row_t r;
	int score;
};
moveRightInfo rightInfo[65536]; 

class board{
public:

	board(){
		m_board = 0;	
	};

	board(board_t pos){
		m_board = pos;	
	};

	~board(){;}

	// initialize the board with two random piece
	void init();

	// initialize the board with given starting condition
	void init(board_t b);


	// 1 for piece num 2
	// 2 for piece num 4
	int getNewPiece();

	// return a new vacant position
	// if no empty pos -> return invalidPos
	int getEmptyPos();

	//
	void insertNewPiece();

	//return true if cannot move anymore
	bool isEnd();

	//return true if it is full
	bool isFull();
	
	//return >= 0 value (eaten score retrieve) if it is a move, and move
	//else return -1
	int canMove(int direct);

	// show the board on the screan
	void showBoard();

	// return the hashKey of the current board
	board_t getCurrentPosition();

	// evaluation function
	double eval();

	// board status
	board_t m_board;
};

void init_ratio_table(){
	COMMON_RATIOS[0] = 1;
	for(int i = 1; i < 16; i++){
		COMMON_RATIOS[i] = COMMON_RATIOS[i-1] * COMMON_RATIO;
	}
	/*
	COMMON_RATIOS[15] = 1;
	for(int i = 14; i >= 0; i--){
		COMMON_RATIOS[i] = COMMON_RATIOS[i+1] * 2;
	}*/
}

void init_tile_score(){
	tile_score[0] = 0;
	tile_score[1] = 2;
	for(int i = 2; i < 16; i++){
		tile_score[i] = tile_score[i-1] * 2;
	}
}

void init_ct_score(){
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j ++){
			ct_score[i*16+j] = COMMON_RATIOS[i]*tile_score[j];
		}
	}
}

void init_score_table(){

	int mask = 0xf;
	for(int num = 0; num < 65536; num++){

		int line_num[4];
		for(int i = 0; i < 4; i++){
			line_num[i] = ((num>>(4*i)) & mask);
		}

		line_score[num][0]	=	tile_score[line_num[0]]		*	COMMON_RATIOS[0]
							+	tile_score[line_num[1]]		*	COMMON_RATIOS[1]
							+	tile_score[line_num[2]]		*	COMMON_RATIOS[2]
							+	tile_score[line_num[3]]		*	COMMON_RATIOS[3];
		line_score[num][1]	=	tile_score[line_num[3]]		*	COMMON_RATIOS[4]
							+	tile_score[line_num[2]]		*	COMMON_RATIOS[5]
							+	tile_score[line_num[1]]		*	COMMON_RATIOS[6]
							+	tile_score[line_num[0]]		*	COMMON_RATIOS[7];
		line_score[num][2]	=	tile_score[line_num[0]]		*	COMMON_RATIOS[8]
							+	tile_score[line_num[1]]		*	COMMON_RATIOS[9]
							+	tile_score[line_num[2]]		*	COMMON_RATIOS[10]
							+	tile_score[line_num[3]]		*	COMMON_RATIOS[11];
		line_score[num][3]	=	tile_score[line_num[3]]		*	COMMON_RATIOS[12]
							+	tile_score[line_num[2]]		*	COMMON_RATIOS[13]
							+	tile_score[line_num[1]]		*	COMMON_RATIOS[14]
							+	tile_score[line_num[0]]		*	COMMON_RATIOS[15];
	}

}

void init_log2(){
	//log2val[65536];
	int num = 1;
	for(int i = 0; i < 65536; i++){
		log2val[i] = 0;
	}
	for(int i = 0; i < 16; i++){
		log2val[num] = i;
		num *= 2;
	}
}

double getScore(board_t b){
	double score = 0;
	//cout << hex << b << endl;
	for(int i = 0; i < 4; i++){
		//cout << hex << getRow(b,3-i) << endl;
		score += line_score[getRow(b,3-i)][i];
		//cout << score << endl;
		//system("pause");
	}
	return score;
}

void init_right_info(){
	for(int i = 0; i < 65536; i++){
		row_t num = i;
		int score = moveRight(num);
		rightInfo[i].r = num;
		rightInfo[i].score = score;
	}
}

void init_MinMaxSum(){
	maxScoreOfSum[0] = 0;
	minScoreOfSum[0] = 0;
	for(int num = 1; num < 32768; num++){
		int finalSum = num * 2;
		int maxVal = 32768;
		//board_t maxBoard = 0;
		//board_t minBoard = 0;
		int pow = 15;
		int count = 0;
		while(1){
			int val = finalSum / maxVal;
			if(val > 0){
				//board_t addVal = pow;
				maxScoreOfSum[num*2] += tile_score[pow] * COMMON_RATIOS[count];
				minScoreOfSum[num*2] += tile_score[pow] * COMMON_RATIOS[15-count];
				//maxBoard = maxBoard | (addVal << 4*count);
				//minBoard = minBoard | (addVal << 4*(15-count));
				finalSum -= maxVal;
				count ++;
				if(finalSum == 0) break;
			}
			maxVal /= 2;
			pow --;
		}
		//maxScoreOfSum[num*2] = getScore(maxBoard);
		//minScoreOfSum[num*2] = getScore(minBoard);
	}
	//cout << maxScoreOfSum[4096] << endl;
	//cout << minScoreOfSum[4096] << endl;
	//system("pause");
}

void init_rowSum(){
	for(int r = 0; r < 65536; r++){
		//int sum = 0;
		for(int i =0; i < 16; i+=4){
			int val = (r >> i) & 0xf;
			rowSum[r] += tile_score[val];
		}
	}
}

row_t getRow(board_t b,int r){
	return (b>>(48-16*r) & 0xffff);
}

row_t getCol(board_t b,int c){
	uint64_t colMask = (0xf000f000f000f000 >>  (c*4) );
	
	b = (b&colMask) << 4*c;
/*	
	cout << hex << b << endl;
	cout << hex << getRow(b,0) << endl;
	cout << hex << getRow(b,1) << endl;
	cout << hex << getRow(b,2) << endl;
	cout << hex << getRow(b,3) << endl;
*/	
	return getRow(b,0) | (getRow(b,1) >> 4) | (getRow(b,2) >> 8) | (getRow(b,3) >> 12);
}

board_t setRow(board_t b, int r, row_t row){
	
	board_t tempRow = row;
	
	
	switch(r){
		case 0: 
			b = (b&0x0000ffffffffffff) | (tempRow << 48ULL); 
			break;
		case 1:
			b = (b&0xffff0000ffffffff) | (tempRow << 32ULL);
			break;
		case 2:
			b = (b&0xffffffff0000ffff) | (tempRow << 16ULL);
			break;
		case 3:
			b = (b&0xffffffffffff0000) | tempRow;
			break;
		default:
			cout << "Invalid row number!" << endl;
	}
	return b;
}

board_t setCol(board_t b,int c, row_t col){
	
	
	board_t tempCol = col;
	
	tempCol = (tempCol&0xf000) << 48ULL
	| (tempCol&0x0f00) << 36ULL 
	| (tempCol&0x00f0) << 24ULL 
	| (tempCol&0x000f) << 12ULL;
	tempCol = tempCol >> 4*c;
	
	cout << hex << tempCol << endl;
	
	switch(c){
		case 0: 
			b = (b&0x0fff0fff0fff0fff) | tempCol; 
			break;
		case 1:
			b = (b&0xf0fff0fff0fff0ff) | tempCol;
			break;
		case 2:
			b = (b&0xff0fff0fff0fff0f) | tempCol;
			break;
		case 3:
			b = (b&0xfff0fff0fff0fff0) | tempCol;
			break;
		default:
			cout << "Invalid col number!" << endl;
	}
	return b;
}

board_t setRows(row_t* rows){
	board_t tempRows[4];
	board_t b = 0;
	for(int i =0; i < 4; i++){
		tempRows[i] = rows[i];
		tempRows[i] = tempRows[i] << (3-i)*16;
		b = b | tempRows[i];
	}
	return b;
}

board_t setCols(row_t* cols){
	board_t tempCols[4];
	board_t b = 0;
	for(int i =0; i < 4; i++){
		tempCols[i] = cols[i];
		tempCols[i] = (tempCols[i]&0xf000) << 48ULL
				| (tempCols[i]&0x0f00) << 36ULL 
				| (tempCols[i]&0x00f0) << 24ULL 
				| (tempCols[i]&0x000f) << 12ULL;
		tempCols[i] = tempCols[i] >> 4*i;
		b = b | tempCols[i];
	}
	return b;
}

row_t reverseRow(row_t r){
	//cout << hex << (r&0x0f00 >> 4) << endl;
	return (r&0xf000) >> 12 | (r&0x0f00) >> 4 | (r&0x00f0)<<4 | (r&0x000f) << 12;
}

int moveRight(row_t& r){
	
	row_t rowMask = 0x000f;
	
	int curCheck = 0;
	int score = 0;
	bool isMove = false;
	
	for(int i = 1; i <= 3; i++){
		
		int checkNum = (r>>4*curCheck)&rowMask;
		int compareNum = (r >> 4*i)&rowMask;
		//cout << checkNum << ' ' << compareNum << endl;
		
		if(compareNum == 0){
			continue;
		}
		else if(checkNum == 0){
			r += compareNum << curCheck*4;
			row_t mask = rowMask << i*4;
			mask = ~mask;
			r = r&mask;
			isMove = true;
		}
		else if(checkNum == compareNum){
			r += (0x0001 << curCheck*4);
			
			row_t mask = rowMask << i*4;
			mask = ~mask;
			
			r = r&mask;
			curCheck++;
			
			score += tile_score[checkNum+1];
			isMove = true;
		}
		else if(checkNum != compareNum){
			curCheck++;
			i = curCheck;
		}
		
		//cout << curCheck << endl;
		//cout << hex << r << endl;
	}
	
	if(isMove == false) return -1;
	
	return score;
} 

void board::init(){
	m_board = 0;
	insertNewPiece();
	insertNewPiece();
}

void board::init(board_t pos){
	m_board = pos;
}

int board::getNewPiece(){
	
	int r = rand()%10;
	if(r == 0) return 2;
	else return 1;
};

int board::getEmptyPos(){

	int empty = 0;
	for(int i =0; i < 64; i+=4){
		if(((m_board >> i) & 0xf ) == 0) empty ++;
	}
	int iStart = rand()%empty;
	
	bool existEmpty = false;
	
	for(int i = 0; i < 16; i++){
		//board_t b = m_board;
		if(((m_board>>(4*(15-i)))&0xf) == 0){
			if(iStart == 0){
				return i;
			}
			iStart --;
			existEmpty = true;
		}else if(i == 15 && existEmpty == false){
			return -1;
		}
		
	}

	return -1;
}

void board::insertNewPiece(){
	int pos = getEmptyPos();
	if(pos == -1){
		cout << "Wrong with getting new empty position" << endl;
		while(1);
	}else{
		board_t num = getNewPiece();
		
		num = num << 4*(15-pos);
		board_t mask = 0x000f;
		mask = mask << 4*(15-pos);
		//cout << pos << endl;
		//cout << hex << num << endl;
		//cout << hex << mask << endl; 
		mask = ~mask;
		//cout << hex << mask << endl; 
		m_board = (m_board&mask) | num;
	}
};

bool board::isEnd(){
	
	for(int i =0; i < 4; i++){
		row_t r = getRow(m_board, i);
		row_t c = getCol(m_board, i);
		for(int j =0; j < 3; j++){
			if(((r>>4*j)&0xf) == ((r>>4*(j+1))&0xf) || ((r>>4*j)&0xf) == 0){
				return false;
			} 
			if(((c>>4*j)&0xf) == ((c>>4*(j+1))&0xf) || ((c>>4*j)&0xf) == 0){
				return false;
			} 
		} 
		if(((r>>12)&0xf) == 0) return false;
		if(((c>>12)&0xf) == 0) return false;
	}
	

	
	return true;
	
}

bool board::isFull(){
	for(int i = 0; i < 16; i ++){
		if(((m_board >> 4*i) & 0xf) == 0) return false;
	}

	return true;
};

// 0 : up
// 1 : right
// 2 : down 
// 3 : left
int board::canMove(int direct){
	
	row_t row[4];
	int score = 0;
	bool notMove = true;
	switch(direct){
		case 0:
			for(int i =0; i < 4; i++){
				row[i] = reverseRow(getCol(m_board,i));
				if(rightInfo[row[i]].score == -1);
				else{
					row_t result = rightInfo[row[i]].r;
					score += rightInfo[row[i]].score;
					row[i] = result;
					notMove = false;
				}
				//score += moveRight(row[i]);	
				row[i] = reverseRow(row[i]);		
			}
			if(notMove) return -1;
			else{
				m_board = setCols(row);
				return score; 
			}
			break;
		case 1:
			for(int i =0; i < 4; i++){
				row[i] = getRow(m_board,i);
				if(rightInfo[row[i]].score == -1);
				else{
					row_t result = rightInfo[row[i]].r;
					score += rightInfo[row[i]].score;
					row[i] = result;
					notMove = false;
				}
				//score += moveRight(row[i]);				
			}
			if(notMove) return -1;
			else{
				m_board = setRows(row);
				return score; 
			}
			break;
		case 2:
			for(int i =0; i < 4; i++){
				row[i] = getCol(m_board,i);
				if(rightInfo[row[i]].score == -1);
				else{
					row_t result = rightInfo[row[i]].r;
					score += rightInfo[row[i]].score;
					row[i] = result;
					notMove = false;
				}
				//score += moveRight(row[i]);		
			}
			if(notMove) return -1;
			else{
				m_board = setCols(row);
				return score; 
			}
			break;
		case 3:
			for(int i =0; i < 4; i++){
				row[i] = reverseRow(getRow(m_board,i));
				if(rightInfo[row[i]].score == -1);
				else{
					row_t result = rightInfo[row[i]].r;
					score += rightInfo[row[i]].score;
					row[i] = result;
					notMove = false;
				}
				row[i] = reverseRow(row[i]);	
			}
			if(notMove) return -1;
			else{
				m_board = setRows(row);
				return score; 
			}
			break;
		default:
			cout << "Wrong direction!" << endl;
			break;
	}	
	
	return score;
};

void board::showBoard(){
	for(int i = 0; i < 4; i++){
		row_t r = getRow(m_board,i);
		for(int j = 3; j >= 0; j--){
			cout << tile_score[((r>>4*j)&0xf)] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

board_t board::getCurrentPosition(){
	return m_board;
}

double board::eval(){


	/*
	const scoreTable::iterator &it = evalPosition.find(m_board);
	if(it != evalPosition.end()){
		return it->second;
	*/
	
	board_t b[8];
	row_t rows[4];
	row_t reverseRows[4];
	row_t oRows[4];
	row_t oReverseRows[4];
	
	for(int i =0; i < 4; i++){
		rows[i] = getRow(m_board,i);
		oRows[3-i] = rows[i];
		reverseRows[i] = reverseRow(rows[i]);
		oReverseRows[3-i] = reverseRows[i];
	}
	
	
	b[0] = m_board;
	b[1] = setRows(reverseRows);
	b[2] = setRows(oRows);
	b[3] = setRows(oReverseRows);
	b[4] = setCols(rows);
	b[5] = setCols(reverseRows);
	b[6] = setCols(oRows);
	b[7] = setCols(oReverseRows);
	
	double maxScore = 0;
	for(int i = 0; i < 8; i++){
		double score = getScore(b[i]);
		if(score > maxScore) maxScore = score;
	}
	/*
	for(int i = 0; i < 2; i++){
		board_t temp = b[i];
		double score = 0;
		for(int j = 0; j < 4 ; j++){
			for(int k = 0; k < 4; k++){
				int val = ((temp>>(j*16+k*4)) & 0xf);
				if(val != 0){
					
					if(j%2 == 0){
						score += ct_score[(4*j+k)*16+val];
						//score += (double)COMMON_RATIOS[4*j+k]*tile_score[val];
					}else{
						//score += (double)COMMON_RATIOS[4*j+3-k]*tile_score[val];
						score += ct_score[(4*j+3-k)*16+val];
					}
				}
			}
		}
		
		if(score > maxScore) maxScore = score;
	}*/
	/*
	int target = 0;
	int maxVal = 0;
	int val[8];
	for(int i = 0; i < 2; i++){
		board_t temp = b[i];
		double score = 0;
			for(int k = 0; k < 4; k++){
				int val = ((temp>>(k*4)) & 0xf);
				//cout << val << endl;
				if(val != 0) score += ct_score[(k)*16+val];
			}
		
		if(score > maxScore){
			maxScore = score;
			target = i;
		}
	}

	board_t temp = b[target];
	for(int j = 1; j < 4 ; j++){
		for(int k = 0; k < 4; k++){
			int val = ((temp>>(j*16+k*4)) & 0xf);
			//cout << val << endl;
			if(val != 0){
				if(j%2 == 0){
					maxScore += ct_score[(4*j+k)*16+val];
				}else{
					maxScore += ct_score[(4*j+3-k)*16+val];
				}
			}
		}
	}*/
	
	//evalPosition[m_board] = maxScore;
	/*
	for(int i =0 ; i < 8; i++){
		evalPosition[b[i]] = maxScore;
	}
	*/
	return maxScore;
		
}


#endif