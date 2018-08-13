#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "vars.h"

FILE *fp;
word words[10];
int numW=0;

//create a word
word *makeWord(char *string){
	word *newWord=(word *) malloc(sizeof(word));
	newWord->str=string;
	newWord->num=1;
	return newWord;
}

//expand words
void expand(){
	word *ptr=words;
	word *newPtr=(word *)realloc(ptr, 2*sizeof(words));
}

//compare function between words on stored strings for qsort() 
int cmpfuncL(const void *a, const void *b){
	word *a1=(word *) a;
	word *b1=(word *) b;
	return strcmp(a1->str, b1->str);
}

//compare function between words on number of repetitions for qsort() 
int cmpfuncN(const void *a, const void *b){
	word *a1=(word *) a;
	word *b1=(word *) b;
	return (a1->num - b1->num);
}

//attempt to open the desired file
int openFile(char *fileName){
	fp=fopen(fileName,"r");
	return (fp==NULL)? 0 : 1;
}

//record the files in words
void read(){
	char c, *tempWord;
	c=tolower(fgetc(fp));
	tempWord=(char *)malloc(sizeof(char)*100000);
	int i=0;

	while(c!=EOF){
		if(isalnum(c)){
			tempWord[i]=c;
			i++;
		}
		else {
			if(isalnum(tempWord[0])){
				if(numW==sizeof(words))
					expand();

				words[numW]=*makeWord(tempWord);
				numW++;
			}
			i=0;
			tempWord=(char *)malloc(sizeof(char)*100000);
		}
		c=tolower(fgetc(fp));
	}
}

//count the repetition of each word, but only change the num in one of the duplicated words.
void summation(){
	if(numW==0)
		return;

	char *str, *temp;
	str=words[0].str;
	int i, j;
	j=0;
	
	for(i=1; i<numW; i++){
		temp=words[i].str;

		if(strcmp(temp, str)==0)
			j++;
		else{
			words[i-1].num+=j;
			str=words[i].str;
			j=0;
		}
	}
	words[i-1].num+=j;
}

//expected O(n)
//really about O(n + 2n log n)
int main(int ac, char** av){
	if(openFile(av[1])==1)
		printf("... Successfully opened\n");
	else {
		printf("... Failed to open\n");
		return 0;
	}
	
	read();	
	word *ptr=words;
	qsort(ptr, numW, sizeof(word), cmpfuncL);
	summation();
	qsort(ptr, numW, sizeof(word), cmpfuncN);

	int i=numW-10;
	if(i<0)
		i=0;
	while(i<numW){
		printf("%s %d\n", words[i].str, words[i].num);
		i++;
	}

	fclose(fp);
	return 1;
}