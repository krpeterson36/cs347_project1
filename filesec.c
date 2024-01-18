#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

#define ERROR "./filesec -e|-d [filename.txt]"

int encrypt(char* encName, char* dest){
    int encFile = open(encName, O_RDONLY);
    int encDest = open(dest, O_CREAT|O_WRONLY|O_TRUNC);
    if(encFile == -1){
        printf("%s", ERROR);
        return -1;
    }
    int temp[1];
    int i;
    i = read(encFile + 100, temp, 1);
    write(encDest, temp, 1);
    while(i != EOF){
        i = read(encFile + 100, temp, 1);
        write(encDest, temp, 1);
        printf("%d\n", i);
    }
    close(encFile);
    close(encDest);
    return 0;
}

int decrypt(char* decName, char* dest){
    int decFile = open(decName, O_RDONLY);
    int decDest = open(dest, O_CREAT|O_WRONLY|O_TRUNC);
    if(decFile == -1){
        printf("%s", ERROR);
        return -1;
    }
    int temp[1];
    int i;
    i = read(decFile - 100, temp, 1);
    write(decDest, temp, 1);
    while(i != EOF){
        i = read(decFile - 100, temp, 1);
        write(decDest, temp, 1);
        printf("%d\n", i);
    }
    close(decFile);
    close(decDest);
    return 0;
    return 0;
}

int main(int argc, char** argv)
{

    char output_file_name[128];    //You may assume that the length of the output file name will not exceed 128 characters.
    int buff = 0;
    if(argv[2] == NULL){
        printf("%s", ERROR);
        return -1;
    }else if(argc != 3){
        printf("%s", ERROR);
        return -1;
    }
    strncpy(output_file_name, argv[2], sizeof(argv[2]) + 1);
    int opt;
    while((opt = getopt(argc, argv, "e:d:")) != -1){
        switch (opt){
            case 'e':
                strcat(output_file_name, "_enc.txt");
                printf("%s\n", output_file_name);
                return encrypt(argv[2], output_file_name);
            case 'd':
                strcat(output_file_name, "_dec.txt");
                return decrypt(argv[2], output_file_name);
                break;
            case '?':
                printf("%s", ERROR);
                return -1;
        }
    }
    return 0;

}
