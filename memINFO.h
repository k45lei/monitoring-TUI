#include <stdio.h>
#include <stdlib.h>

typedef struct{
	long totalKB, usedKB;
	double totalGiB, usedGiB;
} MEMinfo;

void READ_MEM_PROC_LINE(MEMinfo *m);

void calcGiB(MEMinfo *m);
