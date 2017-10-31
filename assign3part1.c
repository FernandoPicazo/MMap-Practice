#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>

#define REAL_PAGE_SIZE 4096

//protypes
void accessSeq(char *filename);
unsigned long physicalMap(unsigned long x);
unsigned long virtualMap(unsigned long x);
unsigned long offset(unsigned long x);

void main(int argc, char *argv[]){
    char *seqFileName; //buffer that will contain sequence file name

    //check to see that user only input 1 argument
    if(argc == 2){
        seqFileName = argv[1];
    }else{
        fprintf(stderr,"Usage: assign3part1 [filepath]\n");
    }
    
    //make sure that the file exists and can be accessed by the user
    if(access(seqFileName, F_OK) == -1){
        fprintf(stderr, "The sequence file either does not exist or cannot be accessed\n");
    }

    accessSeq(seqFileName);
}

void accessSeq(char *filename){
    //Open file
    int fd, fdout, i;
    struct stat st; //this will be filled wit file characteristics
    unsigned long filesize;
    unsigned long mapsize;
    unsigned long *memAccesses, *memWrite;
    
    char *outputFile = "output-part1";


    stat(filename, &st);

    filesize = st.st_size;



    fd = open(filename, O_RDONLY);
    if(fd == -1){
        fprintf(stderr, "fd is invalid, \n errno value %d", strerror(errno));
        exit(-1);
    }

    fdout = open(outputFile,O_RDWR|O_CREAT|O_TRUNC, 0644);
    if(fdout == -1){
        close(fd);
        fprintf(stderr, "fdout is invalid, \n errno value %d", strerror(errno));
        exit(-1);
    }

    if(lseek(fdout, filesize-1, SEEK_SET)==-1){
        exit(-1);
    }
    if(write(fdout,"",1)==-1){
        exit(-1);
    }



    //compute aligned size
    mapsize = (filesize + (REAL_PAGE_SIZE -1)) & ~(REAL_PAGE_SIZE - 1);

    memAccesses = (unsigned long *)mmap(0, mapsize, PROT_READ, MAP_PRIVATE, fd, 0);
    if(memAccesses == MAP_FAILED){
        fprintf(stderr, "mmap function failed on input file. \n");
        close(fd);
        close(fdout);
        exit(-1);
    }

    memWrite = (unsigned long *)mmap(0, mapsize, PROT_READ |PROT_WRITE, MAP_SHARED, fdout,0);
    if(memWrite == MAP_FAILED){
        close(fd);
        close(fdout);
        fprintf(stderr, "Function failed on output file");
        exit(-1);
    }

    


    //Scan traverses from the memory file
    for(i = 0; i < filesize/sizeof(unsigned long);i++){

        unsigned long virtPage = virtualMap(memAccesses[i]);

        unsigned long currOffset = offset(memAccesses[i]);

        unsigned long physPage = physicalMap(virtPage);


        unsigned long x = physPage << 7;

        unsigned long y = x + currOffset;
        

        memWrite[i] = y;

    }


    //finalize on disk
    if(msync(memWrite, mapsize, MS_SYNC) == -1){
        fprintf(stderr, "Could not finalize file to disk\n");
    }

    if(munmap(memAccesses, mapsize)==-1){
        fprintf(stderr, "Unable to unmap a file");
    }
    if(munmap(memWrite, mapsize)==-1){
        fprintf(stderr,"Unable to umap a file");
        close(fd);
        close(fdout);
        exit(-1);
    }

    close(fd);
    close(fdout);    
}


unsigned long physicalMap(unsigned long x){
    switch(x){
        case 0:
            return 2;
        case 1:
            return 4;
        case 2:
            return 1;
        case 3: 
            return 7;
        case 4:
            return 3;
        case 5:
            return 5;
        case 6:
            return 6;
        default: 
            return -1;
    }
}

unsigned long virtualMap(unsigned long x){
    return x >> 7;
}

unsigned long offset(unsigned long x){
    return (x & 0x007f);
}
