#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

typedef struct{
	char vendor[16];
	int cores;
} CPUinfo;

typedef struct{
	uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
	double percent;
} COREinfo;


COREinfo** READ_PROC_LINE(COREinfo **cinfo, FILE *stat){

	if(stat == NULL){
		perror("Invalid FILE error");
		return cinfo;
	}
	
	char buffer[256];
	int core_num = 0;
	
	while (fgets(buffer, sizeof(buffer), stat) != NULL && cinfo != NULL){
		unsigned long long u, n, s, i, w, q, sq, st;
		if(strncmp(buffer, "cpu", 3) == 0 && isdigit((unsigned char)buffer[3])) {
			sscanf(buffer + 5,
			"%llu %llu %llu %llu %llu %llu %llu %llu",
			&u, &n, &s, &i, &w, &q, &sq, &st);
			cinfo[core_num] -> user = u;
			cinfo[core_num] -> nice = n;
			cinfo[core_num] -> system = s;
			cinfo[core_num] -> idle = i;
			cinfo[core_num] -> iowait = w;
			cinfo[core_num] -> irq = q;
			cinfo[core_num] -> softirq = sq;
			cinfo[core_num] -> steal = st; 
			core_num++;
		}
	}
	
	return cinfo;
}

CPUinfo* CORE_COUNT(CPUinfo *cpu){
	FILE *fp = fopen("/proc/cpuinfo", "r");
	if (fp == NULL){
		perror("opening /proc/cpuinfo failed.");
		return cpu;
	}

	char line[256];
	int count = 0;
	while (fgets(line, sizeof(line), fp)){
		if (strncmp(line, "processor", 9) == 0) {
			count++;
		}
	}
	cpu -> cores = count;
	fclose(fp);
	return cpu;	
}

void calc_P(CPUinfo *cpu, COREinfo **first, COREinfo **second){
	unsigned long long dtotal, dbusy, du, dn, ds, di, dw, dq, dsq, dst;
	for (int i = 0; i < cpu->cores; i++){
		du = first[i] -> user - second[i] -> user;
		dn = first[i] -> nice - second[i] -> nice;
		ds = first[i] -> system - second[i] -> system;
		di = first[i] -> idle - second[i] -> idle;
		dw = first[i] -> iowait - second[i] -> iowait;
		dsq = first[i] -> softirq - second[i] -> softirq;
		dst = first[i] -> steal - second[i] -> steal;

		dtotal = du + dn + ds + di + dw + dq + dsq + dst;
		dbusy = dtotal - di - dw;

		second[i] -> percent = (dbusy / dtotal) * 100;
	}
}

int TOTAL_READ(){
	CPUinfo *cpu = malloc(sizeof(CPUinfo));
	cpu = CORE_COUNT(cpu);
	COREinfo **first = malloc((cpu -> cores) * sizeof(COREinfo));
	COREinfo **second = malloc((cpu -> cores) * sizeof(COREinfo));
	FILE *stat_f = fopen("/proc/stat", "r");
	first = READ_PROC_LINE(first, stat_f);
	fclose(stat_f);
	stat_f = NULL;
	sleep(1); //implement with nanosleep()
	FILE *stat_s = fopen("/proc/stat", "r");
	second = READ_PROC_LINE(second, stat_s);
	fclose(stat_s);

	calc_P(cpu, first, second);
}

