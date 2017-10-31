#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>



//this section defines
#define FREE_PAGE       ULONG_MAX
#define NOT_FOUND       ULONG_MAX - 1
#define OS_PAGE         ULONG_MAX - 2


#define RPS             4096        //Defines Real Page Size
#define OFFSET          7           //Page offset
#define MAX             7           //Defines maximum number of pages

//prototypes

void startPageTable();

unsigned long getVirtPage(unsigned long physPage);

unsigned long getPhysPage(unsigned long virtPage);

int assignVirtPage(unsigned long physPage, unsigned long virtPage);

unsigned long getPageTimer(unsigned long physPage);

void pageTimer();


void startPhyPg();

unsigned long getFreePage();

unsigned long removePage();


