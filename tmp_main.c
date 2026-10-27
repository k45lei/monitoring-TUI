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

    COREinfo *cpus = malloc((AFTER+1)*sizeof(COREinfo));
    MEMinfo *mem = malloc(sizeof(MEMinfo));
    READ_PROC_LINE(cpus, true);
    for (int i = 0; i < 30000; i++){
         usleep(usleep_time);
         READ_PROC_LINE(cpus, false);
         CALC_P(cpus);
         READ_MEM_PROC_LINE(mem);
         calcGiB(mem);
         clear();
         int cpu_cols = (int)((((double)cpus[1].percent) / 100.00) * (double)strlen(BANNER_CPU[0]));
         int mem_cols = (int)((mem->usedGiB / mem->totalGiB) * (double)strlen(BANNER_MEMORY[0]));
         print_banner(BANNER_LOGO, N_ROW, DEFAULT);
         print_banner(BANNER_CPU, N_ROW, cpu_cols);
         print_banner(BANNER_MEMORY, N_ROW, mem_cols);
         refresh();
    }

    clear();
    print_banner(BANNER_MEMORY, N_ROW, 40);

    end();
    free(mem);
    free(cpus);
    return 0;
    init();
	
    COREinfo *cpus = malloc((AFTER+1)*sizeof(COREinfo));
    MEMinfo *mem = malloc(sizeof(MEMinfo));
