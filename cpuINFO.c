#include "cpuINFO.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

// int get_CORECOUNT(){
// 	
// 	// gets the core count used for init
// 	
// 	FILE *stat = fopen("/proc/stat", "r");
// 	if (stat == NULL){
// 		perror("failed to open /proc/stat");
// 		return -1;
// 	}
// 	char buffer[256];
// 	int count = 0;
// 	while (fgets(buffer, sizeof(buffer), stat) != NULL){
// 		if(strncmp(buffer, "cpu", 3) == 0 && isdigit((unsigned char)buffer[3]))
// 			count++;
// 	}
// 	return count;
// }


void READ_PROC_LINE(COREinfo *cinfo, bool before){ 

	// reads a single moment in time proc stat for all cores

	FILE *stat = fopen("/proc/stat", "r");
	
	
	char buffer[256];

	int bora;
	
	if (before){
		bora = BEFORE;
	}
	else {
		bora = AFTER;
	}
	if(stat == NULL){
		perror("Invalid FILE error");
		return;
	}
	else {
		unsigned long long u, n, s, i, w, q, sq, st;
		fgets(buffer, sizeof(buffer), stat);
		
		sscanf(buffer + 5,
		"%llu %llu %llu %llu %llu %llu %llu %llu",
		&u, &n, &s, &i, &w, &q, &sq, &st);
		cinfo[bora].user = u;
		cinfo[bora].nice = n;
		cinfo[bora].system = s;
		cinfo[bora].idle = i;
		cinfo[bora].iowait = w;
		cinfo[bora].irq = q;
		cinfo[bora].softirq = sq;
		cinfo[bora].steal = st; 
	} 
	fclose(stat);
	stat = NULL;
	return;
}

void CALC_P(COREinfo *cpu){

	// takes in array of COREinfo struct and calculates the percentage for each

	unsigned long long dtotal, dbusy, du, dn, ds, di, dw, dq, dsq, dst;
	du = cpu[1].user - cpu[0].user;
	dn = cpu[1].nice - cpu[0].nice;
	ds = cpu[1].system - cpu[0].system;
	di = cpu[1].idle - cpu[0].idle;
	dw = cpu[1].iowait - cpu[0].iowait;
	dq = cpu[1].irq - cpu[0].irq;
	dsq = cpu[1].softirq - cpu[0].softirq;
	dst = cpu[1].steal - cpu[0].steal;

	dtotal = du + dn + ds + di + dw + dq + dsq + dst;
	dbusy = dtotal - di - dw;

	cpu[1].percent = (dtotal == 0) ? 0.0 : ((double)dbusy / (double)dtotal) * 100.0;
}
