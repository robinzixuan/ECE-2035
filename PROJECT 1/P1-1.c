/*    This program solves a minesweeper game.

PLEASE FILL IN THESE FIELDS:
Your Name: Haozheng Luo
Date: Sept 26th 2018
 */

#include "minefield.h"

int map[10][10];
// set up 10*10 map
int total = 0;
int FLAG = -1;
int ZERO = 0;
int UNKNOWN = -2;

int getRowNum(int i){
    //get the row num
	int result = 0;
	if(i<3)
		result = -1;
	else if(i>4)
		result = 1;
	else 
		result = 0;
	return result;
}

int getColumnNum(int i){
    //get col num
	int result = 0;
	if(i==0 || i==3 || i== 5)
		result = -1;
	else if(i==2 || i==4 || i== 7)
		result = 1;
	else 
		result = 0;
	return result;
}

int openOrFlag(int r, int c){	
	int openOrFlag_count = 0;
	int tmp = map[r+1][c+1];
	int tmp_arr[8];
	tmp_arr[0] = map[r+1-1][c+1-1];
	tmp_arr[1] = map[r+1-1][c+1];
	tmp_arr[2] = map[r+1-1][c+1+1];
	tmp_arr[3] = map[r+1][c+1-1];
	tmp_arr[4] = map[r+1][c+1+1];
	tmp_arr[5] = map[r+1+1][c+1-1];
	tmp_arr[6] = map[r+1+1][c+1];
	tmp_arr[7] = map[r+1+1][c+1+1];
    // make a new array with 9 number
			
	int flag_sum = 0;
	int num_sum = 0;
	int unknow_sum = 0;
    // init of the value
	for (int i=0; i<8; i++){
		if(tmp_arr[i] == FLAG)
            // if it is already flag
			flag_sum++;
		else if(tmp_arr[i] == UNKNOWN)
            //if it is unknown
			unknow_sum++;
		else
            // if it is open now
			num_sum++;
		}
	if(flag_sum == tmp){
        // if all the mine in the 9 value is flaged
		for (int i=0; i<8; i++){
			int tmp_open = 0;
			if(tmp_arr[i] == UNKNOWN){
				tmp_open = open(r+getRowNum(i), c+getColumnNum(i));
                // open the place
				total++;
				openOrFlag_count++;
				map[r+getRowNum(i)+1][c+getColumnNum(i)+1] = tmp_open;
			}
		}
	}
	if(flag_sum + unknow_sum == tmp){
        // if all the non-mine in the 9 value is open
		for (int i=0; i<8; i++){
			if(tmp_arr[i] == UNKNOWN){
				flag(r+getRowNum(i), c+getColumnNum(i));
				total++;
				openOrFlag_count++;
				map[r+getRowNum(i)+1][c+getColumnNum(i)+1] = FLAG;
			}
		}
	}
	return openOrFlag_count;
}

int openOrFlagAll(){
    // judge whether all the place can open is now open
	int openOrFlagAll_count = 0;
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){	
			openOrFlagAll_count += openOrFlag(i, j);
		}
	}
	return openOrFlagAll_count;
}

void solver(int mn) {
    // final soltion function
    for (int tmpi=0; tmpi<10; tmpi++){
        for (int tmpj=0; tmpj<10; tmpj++){
            if (tmpi ==0 || tmpj == 0 || tmpi == 9 || tmpj == 9){
				map[tmpi][tmpj] = ZERO;  // edge of array
            } else
				map[tmpi][tmpj] = UNKNOWN;
        }
    }
    // original set up array
	
	while(total != 63){
        // get the judgement of end
		for(int i=0; i<8; i++){
			for(int j=0; j<8; j++){	
				if(openOrFlagAll() == 0){
                    // if open all can open
					if(map[i+1][j+1] == UNKNOWN ){
						map[i+1][j+1] = guess(i, j);
                        // guess new one
						total++;
					}
				}
			}
		}	
	}
    display_field_discovered_so_far();
}

