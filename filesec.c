#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

#define ERROR "./filesec -e|-d [filename.txt]\n"

//encrypts the file, returns -1 on failure, 0 on success
int fileEncrypt(char* encName, char* dest){

    //opens the file that we will encrypt
    int encFile = open(encName, O_RDONLY);
    if(encFile == -1){
        printf("%s", ERROR);
        close(encFile);
        return -1;
    }

    //opens the target file
    int encDest = open(dest, O_WRONLY|O_CREAT|O_TRUNC, 00700);
    if(encDest == -1){
        printf("%s", ERROR);
        close(encFile);
        close(encDest);
        return -1;
    }

    //Read's one bit at a time into unEnc, then adds 100 to it and saves it to enc, then writes 1 bit from enc to destFile
    int buffSize = 100; 
    char unEnc[1000];
    char enc[1000];
    /*for(int i = 0; i < buffSize; i++){
        unEnc[i] = 0;
        enc[i] = 0;
    }*/
    int count = 1;
    struct timeval startTime, endTime;
    gettimeofday(&startTime, 0);
    int q;
    int x = 0;
    while((q = read(encFile, unEnc, buffSize)) > 0){ 
        for(int i = x; i < x+q; i++){
            enc[i] = unEnc[i] + 100;
        }
        x = x + q;
        count++;
    }
    write(encDest, enc, x);
    gettimeofday(&endTime, 0);
    long sec = endTime.tv_sec - startTime.tv_sec;
    long milliSec = endTime.tv_usec - startTime.tv_usec;
    printf("Read & Write calls: %d\nIt took %ld seconds and %ld milliseconds\n", count, sec, milliSec);
    close(encFile);
    close(encDest);
    return 0;
}

//decrypts the file, returns -1 on failure, 0 on success
int fileDecrypt(char* decName, char* dest){

    //opens source file
    int decFile = open(decName, O_RDONLY);
    if(decFile == -1){
        printf("%s", ERROR);
        close(decFile);
        return -1;
    }

    //opens target file
    int decDest = open(dest, O_WRONLY|O_CREAT|O_TRUNC, 00700);
    if(decDest == -1){
        printf("%s", ERROR);
        close(decFile);
        close(decDest);
        return -1;
    }

    //reads 1 bit from source file to unDec, decrements unDec by 100 and stores in dec, reads 1 bit from dec to destFile
    int buffSize = 100;
    char unDec[1000];
    char dec[1000];
    /*for(int i = 0; i < 1000; i++){
        unDec[i] = 0;
        dec[i] = 0;
    }*/
    int count = 1;
    struct timeval startTime, endTime;
    gettimeofday(&startTime, 0);
    int q;
    int x = 0;
    while((q = read(decFile, unDec, buffSize)) > 0){ 
        for(int i = x; i < x+q; i++){
            dec[i] = unDec[i] - 100;
        }
        x = x + q;
        count++;
    }
    write(decDest, dec, x);
    gettimeofday(&endTime, 0);
    long sec = endTime.tv_sec - startTime.tv_sec;
    long milliSec = endTime.tv_usec - startTime.tv_usec;
    printf("Read & Write calls: %d\nIt took %ld seconds and %ld milliseconds\n", count, sec, milliSec);
    close(decFile);
    close(decDest);
    return 0;
}

int main(int argc, char** argv)
{

    char output_file_name[128];    //You may assume that the length of the output file name will not exceed 128 characters.

    //Checks to see that there are exactly 3 arguments
    if(argc != 3){
        printf("%s", ERROR);
        return -1;
    }

    //gets the file name
    int i = 0;
    while(argv[2][i] != '.'){
        output_file_name[i] = argv[2][i];
        i++;
    }

    //sets the rest of the array to null
    while(i < 128){
        output_file_name[i] = '\0';
        i++;
    }

    //Determins which flag the program was run with
    int opt;
    while((opt = getopt(argc, argv, "e:d:")) != -1){
        switch (opt){
            case 'e':
                strcat(output_file_name, "_enc.txt");
                return fileEncrypt(argv[2], output_file_name);
                break;
            case 'd':
                strcat(output_file_name, "_dec.txt");
                return fileDecrypt(argv[2], output_file_name);
                break;
            case '?':
                printf("%s", ERROR);
                return -1;
        }
    }

    return 0;

}
