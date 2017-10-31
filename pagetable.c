#include "assign3.h"

static unsigned long PAGE_TABLE[MAX];

static unsigned long PAGE_TIMER[MAX];


void startPageTable(){
    
    unsigned long i;

    PAGE_TABLE[0] = OS_PAGE; //These will likely never be accessed or modified
    PAGE_TIMER[0] = 0;
    for(i=0; i< MAX; i++){
        PAGE_TABLE[i] = FREE_PAGE;
        PAGE_TIMER[i] = 0;
    }
}


unsigned long getVirtPage(unsigned long physPage){
    return PAGE_TABLE[physPage];
}

unsigned long getPhysPage(unsigned long virtPage){
    unsigned long i;
    for(i=1; i< MAX; i++){
        if(PAGE_TABLE[i] == virtPage){
            return i;
        }
    }
    return NOT_FOUND;
}

int assignVirtPage(unsigned long physPage, unsigned long virtPage){
    if(physPage < MAX){
        PAGE_TABLE[physPage] = virtPage;
        PAGE_TIMER[physPage] = 0;
        return 1;
    }
    return 0;
}

unsigned long getPageTimer(unsigned long physPage){
    return PAGE_TIMER[physPage];
}

void pageTimer(){
    unsigned long i;
    for(i=1; i < MAX; i++){
        PAGE_TIMER[i] ++;
    }
}
