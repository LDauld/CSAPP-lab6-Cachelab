#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM 1
typedef struct{
	int vaild;
    int tag;
    int LruNumber;
}Line;
typedef struct{
    Line *lines;
}Set;
typedef struct{
    int set_num;
    int line_num;
    Set *sets;
}Sim_Cache;
int hit=0,miss=0,eviction=0;
void printHelpMenu(){
	printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
	printf("Options:\n");
	printf("  -h         Print this help message.\n");
	printf("  -v         Optional verbose flag.\n");
	printf("  -s <num>   Number of set index bits.\n");
	printf("  -E <num>   Number of lines per set.\n");
	printf("  -b <num>   Number of block offset bits.\n");
	printf("  -t <file>  Trace file.\n\n");
	printf("Examples:\n");
	printf("  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
	printf("  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}
void checkOptarg(int s,int E,int b,char *fileName) {
	/*if (atol(curOptarg)<=0) {
		printf("./csim-ref: Missing required command line argument\n");
		printHelpMenu();
		exit(0);
	}*/
	if (s==0 || E==0 || b==0 || fileName==NULL) {
		printf("/csim: Missing required command line argument\n");
		printHelpMenu();
		exit(0);
	}
}
int get_Opt(int argc,char **argv,int *s,int *E,int *b,char *tracefileName,int *isVerbose) {
	int opt;
	while (-1!=(opt=getopt(argc,argv,"hvs:E:b:t:"))) {
		switch(opt) {
			case 'v':
				*isVerbose=1;
				break;
			case 's':
				*s=atoi(optarg);
				break;
			case 'E':
				*E=atoi(optarg);
				break;
			case 'b':
				*b=atoi(optarg);
				break;
			case 't':
				strcpy(tracefileName,optarg);
				break;
			case 'h':
			default:
				printHelpMenu();
				exit(0);
				break;
		}
	}
	return 1;
}
void init_SimCache(int s,int E,int b,Sim_Cache *cache) {
	cache->set_num=1<<s;
	cache->line_num=E;
	cache->sets=(Set*) malloc (cache->set_num * sizeof(Set));
	int i,j;
	for (i=0;i<cache->set_num;i++) {
		cache->sets[i].lines=(Line*) malloc (E * sizeof(Line));
		for (j=0;j<cache->line_num;j++) {
			cache->sets[i].lines[j].vaild=0;
			cache->sets[i].lines[j].LruNumber=0;
		}
	}
	return;
}
int free_SimCache(Sim_Cache *cache) {
	int i;
	for (i=0;i<cache->line_num;i++)
		free(cache->sets[i].lines);
	free(cache->sets);
	return 1;
}
void putSets(Sim_Cache *cache) {
	int i,j;
	for (i=0;i<cache->set_num;i++) {
		printf("Set %d:\n",i+1);
		for (j=0;j<cache->line_num;j++) {
			printf("Line %d:",j+1);
			printf("vaild:%d\ttag=%d\tLruNumber:%d\n",cache->sets[i].lines[j].vaild,cache->sets[i].lines[j].tag,cache->sets[i].lines[j].LruNumber);
		}
	}
	return;
}
void updateLruNumber(Sim_Cache *sim_cache,int setBits,int hitIndex) {
	sim_cache->sets[setBits].lines[hitIndex].LruNumber=MAX_NUM;
	int i;
	for (i=0;i<sim_cache->line_num;i++) {
		if (i!=hitIndex)
			sim_cache->sets[setBits].lines[i].LruNumber-=1;
	}
	return;
}
int findMinLruNumber(Sim_Cache *sim_cache,int setBits) {
	int i;
	int evi_Index,MinLruNumber;
	evi_Index=0;
	MinLruNumber=sim_cache->sets[setBits].lines[0].LruNumber;
	for (i=0;i<sim_cache->line_num;i++) {
		if (MinLruNumber > sim_cache->sets[setBits].lines[i].LruNumber) {
			evi_Index=i;
			MinLruNumber=sim_cache->sets[setBits].lines[i].LruNumber;
		}
	}
	return evi_Index;
}
int isMiss(Sim_Cache *sim_cache,int setBits,int tagBits) {
	int i;
	int Miss=1;
	for (i=0;i<sim_cache->line_num;i++) {
		if (sim_cache->sets[setBits].lines[i].vaild == 1 && sim_cache->sets[setBits].lines[i].tag == tagBits) {
			Miss=0;
			updateLruNumber(sim_cache,setBits,i);
			break;
		}
	}
	return Miss;
}
int updateCache(Sim_Cache *sim_cache,int setBits,int tagBits) {
	int full;
	int i;
	full=1;
	for (i=0;i<sim_cache->line_num;i++) {
		if (sim_cache->sets[setBits].lines[i].vaild == 0) {
			full=0;
			break;
		}
	}
	if (full==0) {
		sim_cache->sets[setBits].lines[i].vaild=1;
		sim_cache->sets[setBits].lines[i].tag=tagBits;
		updateLruNumber(sim_cache,setBits,i);
	}
	else if (full==1) {
		int evi_Index=findMinLruNumber(sim_cache,setBits);
		sim_cache->sets[setBits].lines[evi_Index].tag=tagBits;
		updateLruNumber(sim_cache,setBits,evi_Index);
	}
	return full;
}
void loadData(Sim_Cache *sim_cache,int setBits,int tagBits,int isVerbose) {
	if (isMiss(sim_cache,setBits,tagBits)) {
		miss++;
		if (isVerbose==1)
			printf("miss ");
		if (updateCache(sim_cache,setBits,tagBits)==1) {
			eviction++;
			if (isVerbose==1)
				printf("eviction ");
		}
	}
	else {
		hit++;
		if (isVerbose==1)
			printf("hit ");
	}
	return;
}
void storeData(Sim_Cache *sim_cache,int setBits,int tagBits,int isVerbose) {
	loadData(sim_cache,setBits,tagBits,isVerbose);
}
void modifyData(Sim_Cache *sim_cache,int setBits,int tagBits,int isVerbose) {
	loadData(sim_cache,setBits,tagBits,isVerbose);
	storeData(sim_cache,setBits,tagBits,isVerbose);
}
int getSet(int addr,int s,int b){
	return ( addr>>b ) & ( (1<<s)-1 );
}
int getTag(int addr,int s,int b){
	return addr>>(s+b);
}
void testbyHand(Sim_Cache *sim_cache,int s,int E,int b,int isVerbose) {
	char ope[2];
	int addr,size;
	while(scanf("%s %x,%d",ope,&addr,&size)!=EOF) {
		//getchar();
		if (ope[0]=='I')
			continue;
		int setBits,tagBits;
		setBits=( addr>>b ) & ( (1<<s)-1 );
		tagBits=addr>>(s+b);
		printf("%s %x,%d ",ope,addr,size);
		if (ope[0]=='L')
			loadData(sim_cache,setBits,tagBits,isVerbose);
		else if (ope[0]=='S')
			storeData(sim_cache,setBits,tagBits,isVerbose);
		else if (ope[0]=='M')
			modifyData(sim_cache,setBits,tagBits,isVerbose);
		printf("\nsetBits=%d\ttagBits=%d\n",setBits,tagBits);
		putSets(sim_cache);
	}
}
int main(int argc,char **argv)
{
	int s=0,E=0,b=0,v=0;
	char fileName[100]="\0";
	Sim_Cache cache;
	get_Opt(argc,argv,&s,&E,&b,fileName,&v);
	checkOptarg(s,E,b,fileName);
	init_SimCache(s,E,b,&cache);
	/*printf("-v = %d\n",v);
	printf("-s = %d\n",s);
	printf("-E = %d\n",E);
	printf("-b = %d\n",b);
	printf("-f = %s\n",fileName);
	testbyHand(&cache,s,E,b,v);*/
	//putSets(&cache);
	FILE *tracefile=fopen(fileName,"r");
	char ope[2];
	int addr,size;
	while(fscanf(tracefile,"%s %x,%d",ope,&addr,&size)!=EOF) {
		if (ope[0]=='I')
			continue;
		int setBits,tagBits;
		setBits=getSet(addr,s,b);
		tagBits=getTag(addr,s,b);
		printf("setBits=%d\ttagBits=%d\t",setBits,tagBits);
		if (v==1)
			printf("%s %x,%d ",ope,addr,size);
		if (ope[0]=='L')
			loadData(&cache,setBits,tagBits,v);
		else if (ope[0]=='S')
			storeData(&cache,setBits,tagBits,v);
		else if (ope[0]=='M')
			modifyData(&cache,setBits,tagBits,v);
		
		//putSets(sim_cache);
		if (v==1)
			printf("\n");
	}
	printSummary(hit,miss,eviction);
	free_SimCache(&cache);
    return 0;
}
