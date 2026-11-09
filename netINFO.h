#ifndef NETINFO_H
#define NETINFO_H

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define BEFORE 0
#define AFTER 1

typedef struct{
	double rx,tx;
}NETinfo;


//reads /proc/net/dev and populates the NETinfo struct passed into it
void readNET(NETinfo *info, bool before);


void becomebeforeNET(NETinfo *info);

//calculates the total traffic going through from the values passed in 
//by the struct
int netTRAFFIC(NETinfo *info);
#endif
