#include "cpuINFO.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>


void READ_PROC_LINE(COREinfo *cpu, bool before){ 

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
		cpu[bora].user = u;
		cpu[bora].nice = n;
		cpu[bora].system = s;
		cpu[bora].idle = i;
		cpu[bora].iowait = w;
		cpu[bora].irq = q;
		cpu[bora].softirq = sq;
		cpu[bora].steal = st; 
	} 
	fclose(stat);
	stat = NULL;
	return;
}

void BECOME_BEFORE(COREinfo *cpu){
	cpu[AFTER].user = cpu[BEFORE].user;
	cpu[AFTER].nice = cpu[BEFORE].nice;
	cpu[AFTER].system = cpu[BEFORE].system;
	cpu[AFTER].idle = cpu[BEFORE].idle;
	cpu[AFTER].iowait = cpu[BEFORE].iowait;
	cpu[AFTER].irq = cpu[BEFORE].irq;
	cpu[AFTER].softirq = cpu[BEFORE].softirq;
	cpu[AFTER].steal = cpu[BEFORE].steal;
}

double CALC_P(COREinfo *cpu){

	// takes in array of COREinfo struct and calculates the percentage for each

	unsigned long long dtotal, dbusy, du, dn, ds, di, dw, dq, dsq, dst;
	du = cpu[AFTER].user - cpu[AFTER].user;
	dn = cpu[AFTER].nice - cpu[AFTER].nice;
	ds = cpu[AFTER].system - cpu[AFTER].system;
	di = cpu[AFTER].idle - cpu[AFTER].idle;
	dw = cpu[AFTER].iowait - cpu[AFTER].iowait;
	dq = cpu[AFTER].irq - cpu[AFTER].irq;
	dsq = cpu[AFTER].softirq - cpu[AFTER].softirq;
	dst = cpu[AFTER].steal - cpu[AFTER].steal;

	dtotal = du + dn + ds + di + dw + dq + dsq + dst;
	dbusy = dtotal - di - dw;

	return (dtotal == 0) ? 0.0 : ((double)dbusy / (double)dtotal) * 100.0;
}

