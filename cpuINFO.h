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
	double percent;
} COREinfo;



void READ_PROC_LINE(COREinfo *cpu, bool before);

void BECOME_BEFORE(COREinfo *cpu);

double CALC_P(COREinfo *cpu);
