#include "assign3.h"
static unsigned long PAGE_LIST[MAX];

void startPhyPg(){
    unsigned long i;

    PAGE_LIST[0] = 1;

    for(i=1; i< MAX; i++){
        PAGE_LIST[i] = 0;
    }
}

unsigned long getFreePage(){
    unsigned long i;
    for(i = 1;i<MAX; i++){
        if(PAGE_LIST[i] == 0){
            PAGE_LIST[i] = 1;
            return i;
        }
    }
    return (unsigned long)-1;
}

unsigned long removePage(){
    unsigned long i, t;
    unsigned long LRU = 0;
    unsigned long physpage = NOT_FOUND;
    for(i=1; i<MAX; i++){
        t = getPageTimer(i);
        if(t>LRU){
            physpage = i;
            LRU = t;
        }
    }
    return physpage;
}


