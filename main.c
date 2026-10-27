#include "cpuINFO.h"
#include "memINFO.h"
#include "textCONST.h"
#include "display.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>


void run(unsigned int usleep_time){	
    COREinfo *cpus = malloc((AFTER+1)*sizeof(COREinfo));
    MEMinfo *mem = malloc(sizeof(MEMinfo));
    READ_PROC_LINE(cpus, true);
    double cpu_percent;
    while(true){
         usleep(usleep_time);
         READ_PROC_LINE(cpus, false);
         cpu_percent = CALC_P(cpus);
         BECOME_BEFORE(cpus);
         READ_MEM_PROC_LINE(mem);
         calcGiB(mem);
         erase();
         int cpu_cols = (int)((((double)cpu_percent) / 100.00) * (double)strlen(BANNER_CPU[0]));
         int mem_cols = (int)((mem->usedGiB / mem->totalGiB) * (double)strlen(BANNER_MEMORY[0]));
         print_banner(BANNER_LOGO, N_ROW, DEFAULT);
         print_banner(BANNER_CPU, N_ROW, cpu_cols);
         print_banner(BANNER_MEMORY, N_ROW, mem_cols);
         refresh();
	}
    free(mem);
    free(cpus);
}


int main(int argc, char *argv[]) {
    unsigned int usleep_time = 10000; // default 10ms
    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--usleep") == 0) && i + 1 < argc) {
            usleep_time = (unsigned int)strtoul(argv[i + 1], NULL, 10);
            i++; // skip next
        }
    }
    init();

	run(usleep_time);
	
    end();
    return 0;
}
