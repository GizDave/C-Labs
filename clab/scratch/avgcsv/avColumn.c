#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "variables.h"

FILE *file;
struct shell *hanger;

//create a node
struct node *alloc_node(double value){
	struct node *tempNode=(struct node *)malloc(sizeof(struct node));
	tempNode->value=value;
	tempNode->next=NULL;
	return tempNode;
}

//create a shell
struct shell *alloc_shell(){
	struct shell *tempshell=(struct shell *)malloc(sizeof(struct shell));
	tempshell->firstNode=NULL;
	tempshell->next=NULL;
	return tempshell;
}

//add node to the end of a node-list
void linkNode(struct node *head, struct node *tail){
	if(head==NULL || tail==NULL)
		return;

	struct node *tempHead=head;
	while(tempHead->next!=NULL)
		tempHead=tempHead->next;
	tempHead->next=tail;
}

//add shell to the end of a shell-list 
void linkShell(struct shell *head, struct shell *tail){
	if(head==NULL || tail==NULL)
		return;

	struct shell *tempHead=head;
	while(tempHead->next!=NULL)
		tempHead=tempHead->next;
	tempHead->next=tail;
}

//attempt to open a file. if failed, print an error
int openFile(char *fileName){
	char *extension=strrchr(fileName,'.');

	if(extension==NULL || strcmp(extension+1,"csv")!=0)
		return 0;
	else{
		file=fopen(fileName, "r");
		if(file==NULL)
			return 0;
	}
	return 1;
}

//read a file and create a data structure to store the values read
void read(){
	hanger=(struct shell *) malloc(sizeof(struct shell));
	char line[1000];
	double *value=(double *) malloc(sizeof(double));
	struct shell *tempShell=hanger;
	struct node *tempNode; //header
	char *token=(char *) malloc(sizeof(char));
	char splitC=';';
	char *delimiter=&splitC;

	while(fgets(line,1000,file)!=NULL){
		printf("%s\n",line);
		
		tempShell->firstNode=alloc_node(0);
		tempNode=tempShell->firstNode;

		token=strtok(line,delimiter); 
		while(token!=NULL){
			sscanf(token,"%lf",value);
			linkNode(tempNode, alloc_node(*value)); 
			token=strtok(NULL,delimiter); 
			memset(value,0,sizeof(double));
		}
		
		tempShell->firstNode=tempNode->next;
		tempShell->next=alloc_shell();
		tempShell=tempShell->next;

		tempNode=alloc_node(0); //header

		memset(line,0,sizeof(char)*200);
	}
}

//read the first column and return a list of nodes that contains the values
struct node *getList(struct shell *head){
	if(head==NULL)
		return NULL;

	//problem below
	struct node *header=alloc_node(0);
	struct node *tempNode=(struct node *) malloc(sizeof(struct node));
	struct shell *tempShell=head;
	while(tempShell!=NULL){
		if(tempShell->firstNode!=NULL){
			tempNode=tempShell->firstNode;
			linkNode(header,tempNode);
			tempShell->firstNode=tempNode->next;
			tempNode->next=NULL;
		}
		tempShell=tempShell->next;
	}
	return header->next;
} 

//average the values of the sum of values of the node list
double average(struct node *head){
	if(head==NULL)
		return 0;

	double sum=0;
	double num=0;

	while(head!=NULL){
		num++;
		sum+=head->value;
		head=head->next;
	}
	return sum/num;
}
/*********************************************************************/
int main(int argc, char *argv[]){
	int i, j, k;
	for(i=1;i<argc;i++){
		j=openFile(argv[i]);
		
		if(j=0)
			continue;
		else{
			read();
			
			struct node *list=getList(hanger);
			while(list!=NULL){
				printf(	"%lf; ",average(list));
				list=getList(hanger);
			}
			printf("\n");
			fclose(file);
			free(hanger);
		}
	}
	return 0;
}
