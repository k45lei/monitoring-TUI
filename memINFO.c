#include <stdio.h>
#include <stdlib.h>
typedef struct{
	long totalKB, usedKB;
	double totalGiB, usedGiB;
} MEMinfo;

void READ_MEM_PROC_LINE(MEMinfo *m){
	FILE *mi = fopen("/proc/meminfo", "r");	

	if(mi == NULL){
		perror("error opening /proc/meminfo");
		return;
	}

	char buffer[256];
	long readinNUM;
	// fgets is line 1 since freshly opened
	fgets(buffer, sizeof(buffer), mi);
	sscanf(buffer + 9, "%ld", &readinNUM);
	m -> totalKB = readinNUM;
	fgets(buffer, sizeof(buffer), mi); //skip free line
	fgets(buffer, sizeof(buffer), mi);
	sscanf(buffer + 13, "%ld", &readinNUM);
	m -> usedKB = m -> totalKB - readinNUM;

	
	fclose(mi);
	mi = NULL;
	return;
}

void calcGiB(MEMinfo *m){
	m -> totalGiB = (double)(m -> totalKB) / (1024 * 1024);
	m -> usedGiB = (double)(m -> usedKB) / (1024 * 1024);
}

// 
// int main(){
// 	MEMinfo *m = malloc(sizeof(MEMinfo));
// 	READ_MEM_PROC_LINE(m);
// 	calcGiB(m);
// 	printf("%.1f \n", m->totalGiB);
// 	printf("%.1f", m->usedGiB);
// 	return 0;
// }


// MemTotal:       28072384 kB
// MemFree:        17414816 kB
// MemAvailable:   21815048 kB
