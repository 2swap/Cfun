#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double speed = 0;

int loop(int t, double offset){
	return (int)((sin(t/speed+offset)+1)*3);
}

int rgbToInt(int t){
	double pi = 3.14159265359;
	if(t == -1)return 232;
	return 16+loop(t,0)*36+loop(t,2*pi/3)*6+loop(t,4*pi/3);
}

int main(int argc, char* argv []){
	double ox, oy;
	int rainbow;
	if(argc != 5){
		ox = -0.7925404000002;
		oy = 0.16076317720001;
		rainbow = 1;
		speed = 6;
	}
	else{
		ox = atof(argv[1]), oy = atof(argv[2]);
		rainbow = strcmp(argv[3], "rainbow");
		speed = atoi(argv[4]);
	}
	int height = 48, width = 72;
	double zoom = 10;
	int duration = 1200;

	int** storage = (int**)malloc(sizeof(int*)*height);
	for(int y = 0; y < height; y++)
		storage[y] = (int*)malloc(sizeof(int)*width);

	for(int k=0;k<duration;k++){
		int maxDivergeCount = 0, minDivergeCount = 10000;
		for(int i=0;i<height;i++){
		   	for(int j=0;j<width;j++){
		   		double iPart = (i - height / 2.) * (4. / zoom) + oy;
		   		double nPart = (j - width / 2.) * (4. / zoom) + ox;
				double editI = iPart;
				double editN = nPart;
				int diverged = 0;
				double divergeCount = 0;
				
				while (!diverged) {
					double newN = editN * editN - editI * editI;
					double newI = editI * editN * 2;
					editI = newI + iPart;
					editN = newN + nPart;
					divergeCount++;
					if (editI * editI + editN * editN > 4){
						diverged = 1;
				    	maxDivergeCount = divergeCount > maxDivergeCount?divergeCount:maxDivergeCount;
				    	minDivergeCount = divergeCount < minDivergeCount?divergeCount:minDivergeCount;
					}
					if (divergeCount > 50 + k*8){
						divergeCount = -1;
						break;
					}
				}
		    	storage[i][j] = divergeCount;
		    }
		}
		for(int i = 0; i < height; i++){
			for(int j = 0; j < width; j++){
				const char base_string[] = "\e[38;5;";
				char out_string [20];
				int number = 0;
				// if(rainbow==0){
				// 	double interp = (storage[i][j]-minDivergeCount)*23./(maxDivergeCount-minDivergeCount);
				// 	number = (interp<23?(int)interp:23)+(255-23);
				// }else{
				 	number = rgbToInt(storage[i][j]);
				// }
				sprintf(out_string, "%s%d%s", base_string, number, "m\u2588");
		    	printf("%s%s",out_string,out_string);
			}
		    printf("\n");
		}
		printf("\e[1;1H\e[2J");
		zoom*=1.03;
	}

	for(int y = 0; y < height; y++)
		free(storage[y]);
	free(storage);

	if(argc != 5){
		printf("arguments: x, y, rainbow, speed\n");
		printf("sample arguments you just saw: %f %f %d %f\n",ox,oy,rainbow,speed);
	}
	return 0;
}
