#define _XOPEN_SOURCE 700          /* for usleep */
#include "cpuINFO.h"
#include "memINFO.h"      /* needed for linking (unused here) */
#include "textCONST.h"    /* needed for linking (unused here) */
#include "display.h"      /* needed for linking (unused here) */
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* -----------------------------------------------------------------
 * Simple argument parser – only –t / --usleep is recognized.
 * -----------------------------------------------------------------*/
static unsigned int parse_sleep_interval(int argc, char *argv[])
{
    unsigned int us = 100000;          /* default 100 ms */
    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "-t") == 0 ||
             strcmp(argv[i], "--usleep") == 0) && i + 1 < argc) {
            us = (unsigned int)strtoul(argv[i + 1], NULL, 10);
            ++i;
        }
    }
    return us;
}

/* -----------------------------------------------------------------
 * Main test loop – identical in spirit to the loop in main.c.
 * -----------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    unsigned int interval_us = parse_sleep_interval(argc, argv);

    /* Allocate a 2‑element COREinfo array (BEFORE, AFTER). */
    COREinfo *cpu = malloc((AFTER + 1) * sizeof(COREinfo));
    if (!cpu) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    /* Initial snapshot – becomes the “BEFORE” reference. */
    READ_PROC_LINE(cpu, true);

    while (1) {
        /* Wait the configured sampling interval. */
        {
        struct timespec ts;
        ts.tv_sec = interval_us / 1000000;
        ts.tv_nsec = (interval_us % 1000000) * 1000;
        nanosleep(&ts, NULL);
    }

        /* Capture the “AFTER” snapshot. */
        READ_PROC_LINE(cpu, false);

        /* Compute utilisation for the elapsed interval. */
        double usage = CALC_P(cpu);
        printf("CPU usage: %.2f%%\n", usage);
        fflush(stdout);

        /* Prepare for the next iteration – shift AFTER to BEFORE. */
        BECOME_BEFORE(cpu);
    }

    /* Cleanup (unreachable in this infinite loop, but kept for completeness). */
    free(cpu);
    return EXIT_SUCCESS;
}
