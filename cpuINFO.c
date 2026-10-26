#include "cpuINFO.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

int get_CORECOUNT(){
	
	// gets the core count used for init
	
	FILE *stat = fopen("/proc/stat", "r");
	if (stat == NULL){
		perror("failed to open /proc/stat");
		return -1;
	}
	char buffer[256];
	int count = 0;
	while (fgets(buffer, sizeof(buffer), stat) != NULL){
		if(strncmp(buffer, "cpu", 3) == 0 && isdigit((unsigned char)buffer[3]))
			count++;
	}
	return count;
}


void READ_PROC_LINE(COREinfo *cinfo, int core_count, bool before){ 

	// reads a single moment in time proc stat for all cores

	FILE *stat = fopen("/proc/stat", "r");
	
	
	char buffer[256];
	int core_num = 0;
	
	if (before){
		if(stat == NULL){
			perror("Invalid FILE error");
			return;
		}
		while (fgets(buffer, sizeof(buffer), stat) != NULL && core_num < core_count){
			unsigned long long u, n, s, i, w, q, sq, st;
			if(strncmp(buffer, "cpu", 3) == 0 && isdigit((unsigned char)buffer[3])) {
				sscanf(buffer + 5,
				"%llu %llu %llu %llu %llu %llu %llu %llu",
				&u, &n, &s, &i, &w, &q, &sq, &st);
				cinfo[core_num].user = u;
				cinfo[core_num].nice = n;
				cinfo[core_num].system = s;
				cinfo[core_num].idle = i;
				cinfo[core_num].iowait = w;
				cinfo[core_num].irq = q;
				cinfo[core_num].softirq = sq;
				cinfo[core_num].steal = st; 
				core_num++;
			}
		}
	}
	else if (!before){
		if(stat == NULL){
			perror("Invalid FILE error");
			return;
		}
		
		while (fgets(buffer, sizeof(buffer), stat) != NULL && core_num < core_count){
			unsigned long long u, n, s, i, w, q, sq, st;
			if(strncmp(buffer, "cpu", 3) == 0 && isdigit((unsigned char)buffer[3])) {
				sscanf(buffer + 5,
				"%llu %llu %llu %llu %llu %llu %llu %llu",
				&u, &n, &s, &i, &w, &q, &sq, &st);
				cinfo[core_num].user1 = u;
				cinfo[core_num].nice1 = n;
				cinfo[core_num].system1 = s;
				cinfo[core_num].idle1 = i;
				cinfo[core_num].iowait1 = w;
				cinfo[core_num].irq1 = q;
				cinfo[core_num].softirq1 = sq;
				cinfo[core_num].steal1 = st; 
				core_num++;
			}
		}
	} 
	fclose(stat);
	stat = NULL;
	return;
}

void CALC_P(COREinfo *cpu, int core_count){

	// takes in array of COREinfo struct and calculates the percentage for each

	unsigned long long dtotal, dbusy, du, dn, ds, di, dw, dq, dsq, dst;
	for (int i = 0; i < core_count; i++){
		du = cpu[i].user1 - cpu[i].user;
		dn = cpu[i].nice1 - cpu[i].nice;
		ds = cpu[i].system1 - cpu[i].system;
		di = cpu[i].idle1 - cpu[i].idle;
		dw = cpu[i].iowait - cpu[i].iowait;
		dsq = cpu[i].softirq1 - cpu[i].softirq;
		dst = cpu[i].steal1- cpu[i].steal;

		dtotal = du + dn + ds + di + dw + dq + dsq + dst;
		dbusy = dtotal - di - dw;

		cpu[i].percent = (dbusy / dtotal) * 100;
	}
}

