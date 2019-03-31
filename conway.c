#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void p(int number){
	const char base_string[] = "\e[38;5;";
	char out_string [20];
	sprintf(out_string, "%s%d%s", base_string, number, "m\u2588");
	printf("%s%s",out_string,out_string);
}

void c(){
	printf("\e[1;1H\e[2J");
}

int main(int argc, char* argv []){
	
	int height = 48, width = 48;
	int duration = 240000;

	int** board = (int**)malloc(sizeof(int*)*height);
	for(int t = 0; t < height; t++)
		board[t] = (int*)malloc(sizeof(int)*width);

	for(int y=0;y<height;y++)
	   	for(int x=0;x<width;x++)
			board[y][x] = (x & y * x + y) % 5 % 2;
	
	for(int k=0;k<duration;k++){
		
		int** board2 = (int**)malloc(sizeof(int*)*height);
		for(int t = 0; t < height; t++)
			board2[t] = (int*)malloc(sizeof(int)*width);
		
		
		for(int y=0;y<height;y++)
		   	for(int x=0;x<width;x++){
				if(x==0||y==0||x==width-1||y==height-1){
					board2[y][x]=0;
					continue;
				}
				int sum = 0;
				for(int dx = -1; dx < 2; dx++)
					for(int dy = -1; dy < 2; dy++)
						if((dx != 0 || dy != 0) && board[y+dy][x+dx]) sum++;
				board2[y][x] = (sum == 3 || (sum == 2 && board[y][x]));
		    }
		
		
		
		c();
		if(k % 400 == 0)
			for(int y=0;y<height;y++){
			   	for(int x=0;x<width;x++){
					board[y][x] = board2[y][x];
					p(255-(board[y][x]?0:23));
				}
				printf("\n");
			}
		
		
		for(int y = 0; y < height; y++)
			free(board2[y]);
		free(board2);
	}
	
	
	
	
	for(int y = 0; y < height; y++)
		free(board[y]);
	free(board);
}
