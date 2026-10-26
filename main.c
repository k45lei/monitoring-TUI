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


int main(void) {
    init();

    int core_count = get_CORECOUNT();
    COREinfo *cpus = calloc(core_count, sizeof(COREinfo));
    READ_PROC_LINE(cpus, core_count, true);
    usleep(100000);
    READ_PROC_LINE(cpus, core_count, false);
    CALC_P(cpus, core_count);

    MEMinfo mem;
    READ_MEM_PROC_LINE(&mem);
    calcGiB(&mem);

    double avg = 0;
    for (int i = 0; i < core_count; i++) avg += cpus[i].percent;
    avg /= core_count;

    int cpu_cols = (int)((avg / 100.0) * (double)strlen(BANNER_CPU[0]));
    int mem_cols = (int)((mem.usedGiB / mem.totalGiB) * (double)strlen(BANNER_MEMORY[0]));

    print_banner(BANNER_LOGO, 6, 0);
    print_banner(BANNER_CPU, 6, cpu_cols);
    print_banner(BANNER_MEMORY, 6, mem_cols);

    end();
    free(cpus);
    return 0;
}
