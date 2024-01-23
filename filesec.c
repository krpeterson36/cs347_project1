#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

#define ERROR "./filesec -e|-d [filename.txt]\n"
#define BUFFER_SIZE 1000

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

    //Reads BUFFER_SIZE amount of bytes into the buffer, then encrypts those bytes, then writes those bytes into the destination file. Repeats until no bytes left to read.
    char buf[BUFFER_SIZE];
    int count = 0;
    struct timeval startTime, endTime;
    gettimeofday(&startTime, 0);
    int q;
    while((q = read(encFile, buf, BUFFER_SIZE)) > 0){ 
        for(int i = 0; i < q; i++){
            buf[i] = buf[i] + 100;
        }
        write(encDest, buf, q);
        count += 2;
    }
    gettimeofday(&endTime, 0);
    long startT = (startTime.tv_sec * 1000000) + startTime.tv_usec;
    long endT = (endTime.tv_sec * 1000000) + endTime.tv_usec;
    long diff = endT - startT;
    printf("Read & Write calls: %d\nIt took %ld microseconds\n", count, diff);
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

    //Reads BUFFER_SIZE amount of bytes into the buffer, then decrypts those bytes, then writes those bytes into the destination file. Repeats until no bytes left to read.
    char buf[BUFFER_SIZE];
    int count = 0;
    struct timeval startTime, endTime;
    gettimeofday(&startTime, 0);
    int q;
    while((q = read(decFile, buf, BUFFER_SIZE)) > 0){ 
        for(int i = 0; i < q; i++){
            buf[i] = buf[i] - 100;
        }
        write(decDest, buf, q);
        count += 2;
    }
    gettimeofday(&endTime, 0);
    long startT = (startTime.tv_sec * 1000000) + startTime.tv_usec;
    long endT = (endTime.tv_sec * 1000000) + endTime.tv_usec;
    long diff = endT - startT;
    printf("Read & Write calls: %d\nIt took %ld microseconds\n", count, diff);
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
