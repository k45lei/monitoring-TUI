#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#define BEFORE 0
#define AFTER 1

typedef struct{
	uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
	uint64_t user1, nice1, system1, idle1, iowait1, irq1, softirq1, steal1;
	double percent;
} COREinfo;

// int get_CORECOUNT();


void READ_PROC_LINE(COREinfo *cinfo, bool before);

void BECOME_BEFORE(COREinfo *cinfo);

void CALC_P(COREinfo *cpu);
