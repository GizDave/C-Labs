#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include "Variables.h"

//extern variable
unsigned int n;
FILE *fp;

//return life state of right neighbor
int getR(int i, int j, int arr[n][n]){
	return arr[i][(j+1)%n];
}

//return life state of left neighbor
int getL(int i, int j, int arr[n][n]){
	return arr[i][(n+j-1)%n];
}

//return life state of top neighbor
int getT(int i, int j, int arr[n][n]){
	return arr[(i+n-1)%n][j];
}

//return life state of bottom neighbor
int getB(int i, int j, int arr[n][n]){
	return arr[(i+1)%n][j];
}

//return life state of top-left neighbor
int getTL(int i, int j, int arr[n][n]){
	return arr[(n+i-1)%n][(n+j-1)%n];
}
//return life state of top-right neighbor
int getTR(int i, int j, int arr[n][n]){
	return arr[(i+n-1)%n][(j+1)%n];
}

//return life state of bottom-left neighbor
int getBL(int i, int j, int arr[n][n]){
	return arr[(i+1)%n][(n+j-1)%n];
}

//return life state of bottom-right neighbor
int getBR(int i, int j, int arr[n][n]){
	return arr[(i+1)%n][(j+1)%n];
}

//return the number of alive neighbors of a given index
int getLives(int i, int j, int arr[n][n]){
	return getL(i,j,arr)+getR(i,j,arr)+getT(i,j,arr)+getB(i,j,arr)\
		+getTR(i,j,arr)+getBR(i,j,arr)+getTL(i,j,arr)+getBL(i,j,arr);
}

//return the state of a given array element in the new generation
int isAlive(int i, int j, int arr[n][n]){
	int neighborsA=getLives(i,j,arr);

	if(arr[i][j]==0)
		return (neighborsA==3)? 1:0;
	else
		return (neighborsA==2 || neighborsA==3)? 1:0;
}

//try opening file and set n
int openFile(char *fileName){
	fp=fopen(fileName,"r");

	if(fp==NULL) 
		return 0;

	n=0;
	
	FILE *tempF=fopen(fileName, "r");
	char c=fgetc(tempF);
	while(c!=EOF && c !='\n'){
		n++;
		c=fgetc(tempF);
	}
	fclose(tempF);

	return 1;
}

//write each character in the file to arr (1-alive; 0-dead)
void read(int arr[n][n]){
	int column, row;
	column=row=0;
	char c=fgetc(fp);

	while(c!=EOF){
		if(c=='.')
			arr[row][column]=0;
		else 
			arr[row][column]=1;

		c=fgetc(fp);
		column++;
		
		if(c=='\n'){
			column=0;
			row++;
			c=fgetc(fp);
		}
	}
}

void nextGen(int arr[n][n]){
	int i,j;
	int temp[n][n];

	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			temp[i][j]=isAlive(i, j, arr);
	
	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			arr[i][j]=temp[i][j];
}

int main(int argc, char **argv){
	if(openFile(argv[1])==0)
		printf("Unable to open file");
	else{
		int arr[n][n];
		int i,j,k;
			
		read(arr);
		
		printf("<Generation 0>\n");
		for(i=0;i<n;i++){
			for(j=0;j<n;j++)
				if(arr[i][j]==1)	
					printf("%c",'x');
				else
					printf("%c",'.');
				printf("\n");
		}
		printf("\n");

		if(argv[2]==NULL){
			printf("No ticks\n");
			return 0;
		}
		
		int generations=atoi(argv[2]);
		for(k=0;k<generations;k++){
			nextGen(arr);
			
			printf("<Generation %d>\n", k+1);
			for(i=0;i<n;i++){
				for(j=0;j<n;j++)
					if(arr[i][j]==1)	
						printf("%c",'x');
					else
						printf("%c",'.');
				printf("\n");
			}
			printf("\n");
		}
		fclose(fp);
	}
	return 0;
}
