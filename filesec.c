#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

#define ERROR "./filesec -e|-d [filename.txt]\n"

int encrypt(char* encName, char* dest){
    int encFile = open(encName, O_RDONLY);
    if(encFile == -1){
        printf("%s", ERROR);
        close(encFile);
        return -1;
    }
    int encDest = open(dest, O_WRONLY|O_CREAT|O_TRUNC, 00700);
    if(encDest == -1){
        printf("%s", ERROR);
        close(encFile);
        close(encDest);
        return -1;
    }
    int unEnc[1];
    int enc[1];
    while(read(encFile, unEnc, 1) > 0){ 
       enc[0] = unEnc[0] + 100;
       write(encDest, enc, 1);
    }
    close(encFile);
    close(encDest);
    return 0;
}

int decrypt(char* decName, char* dest){
    int decFile = open(decName, O_RDONLY);
    if(decFile == -1){
        printf("%s", ERROR);
        return -1;
    }
    int decDest = open(dest, O_WRONLY|O_CREAT|O_TRUNC, 00700);
    if(decDest == -1){
        printf("%s", ERROR);
        return -1;
    }
    int unDec[1];
    int dec[1];
    while(read(decFile, unDec, 1) > 0){
        dec[0] = unDec[0]-100;
        write(decDest, dec, 1);
    }
    close(decFile);
    close(decDest);
    return 0;
}

int main(int argc, char** argv)
{

    char output_file_name[128];    //You may assume that the length of the output file name will not exceed 128 characters.
    if(argc != 3){
        printf("%s", ERROR);
        return -1;
    }
    int i = 0;
    while(argv[2][i] != '.'){
        output_file_name[i] = argv[2][i];
        i++;
    }
    while(i < 128){
        output_file_name[i] = '\0';
        i++;
    }
    int opt;
    while((opt = getopt(argc, argv, "e:d:")) != -1){
        switch (opt){
            case 'e':
                strcat(output_file_name, "_enc.txt");
                printf("%s\n", output_file_name);
                return encrypt(argv[2], output_file_name);
                break;
            case 'd':
                strcat(output_file_name, "_dec.txt");
                printf("%s\n", output_file_name);
                return decrypt(argv[2], output_file_name);
                break;
            case '?':
                printf("%s", ERROR);
                return -1;
        }
    }
    return 0;

}
