#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

#define ERROR "./filesec -e|-d [filename.txt]"

void encrypt(char* fileName){
    FILE destination;
}

void decrypt(char* fileName){

}

int main(int argc, char** argv)
{

    char output_file_name[128];    //You may assume that the length of the output file name will not exceed 128 characters.
    int opt;
    while((opt = getopt(argc, argv, "e:d:")) != -1){
        switch (opt){
            case 'e':
                encrypt();
                break;
            case 'd':
                decrypt();
                break;
            case '?':
                printf("./filesec -e|-d [filename.txt]");
                return -1;
        }
    }
    return 0;

}
