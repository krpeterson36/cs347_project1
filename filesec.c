#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

#define ERROR "Usage:\n./filesec -e|-d [filename.txt]\n"
#define BUFFER_SIZE (1000)

/*Encrypts the file
 *Takes the name of the input file and the name of the output file as parameters
 *returns -1 on failure, 0 on success
 */
int file_encrypt(char* encrypt_name, char* dest){

    //opens the file that we will encrypt
    int encrypt_file = open(encrypt_name, O_RDONLY);
    if(encrypt_file == -1){
        write(1, ERROR, sizeof(ERROR));
        close(encrypt_file);
        return -1;
    }

    //opens the target file
    int encrypt_destination = open(dest, O_WRONLY|O_CREAT|O_TRUNC, 00700);
    if(encrypt_destination == -1){
        write(1, ERROR, sizeof(ERROR));
        close(encrypt_file);
        close(encrypt_destination);
        return -1;
    }

    /*Reads BUFFER_SIZE amount of bytes into the buffer and then encrypts those characters. 
     *After that it writes those bytes into the destination file. Repeats until no bytes left to read.
     */
    char buf[BUFFER_SIZE];
    int count = 0;
    struct timeval start_time, end_time;
    gettimeofday(&start_time, 0);
    int q;
    while((q = read(encrypt_file, buf, BUFFER_SIZE)) > 0){ 
        for(int i = 0; i < q; i++){
            buf[i] = buf[i] + 100;
        }
        write(encrypt_destination, buf, q);
        count += 2;
    }

    //does time calculations for the experiment
    gettimeofday(&end_time, 0);
    long start_time_micro = (start_time.tv_sec * 1000000) + start_time.tv_usec;
    long end_time_micro = (end_time.tv_sec * 1000000) + end_time.tv_usec;
    long diff = end_time_micro - start_time_micro;
    printf("Read & Write calls: %d\nIt took %ld microseconds\n", count, diff);

    //closes the files
    close(encrypt_file);
    close(encrypt_destination);
    return 0;
}

/*Decrypts the file
 *Takes the name of the input file and the name of the output file as parameters
 *returns -1 on failure, 0 on success
 */
int file_decrypt(char* decrypt_file_source, char* dest){

    //opens source file
    int decrypt_file_source_name = open(decrypt_file_source, O_RDONLY);
    if(decrypt_file_source_name == -1){
        write(1, ERROR, sizeof(ERROR));
        close(decrypt_file_source_name);
        return -1;
    }

    //opens target file
    int decrypt_file_destination = open(dest, O_WRONLY|O_CREAT|O_TRUNC, 00700);
    if(decrypt_file_destination == -1){
        write(1, ERROR, sizeof(ERROR));
        close(decrypt_file_source_name);
        close(decrypt_file_destination);
        return -1;
    }

    /*Reads BUFFER_SIZE amount of bytes into the buffer and then decrypts those characters. 
     *After that it writes those bytes into the destination file. Repeats until no bytes left to read.
     */    
    char buf[BUFFER_SIZE];
    int count = 0;
    struct timeval start_time, end_time;
    gettimeofday(&start_time, 0);
    int q;
    while((q = read(decrypt_file_source_name, buf, BUFFER_SIZE)) > 0){ 
        for(int i = 0; i < q; i++){
            buf[i] = buf[i] - 100;
        }
        write(decrypt_file_destination, buf, q);
        count += 2;
    }

    //does time calculations for experimen
    gettimeofday(&end_time, 0);
    long start_time_micro = (start_time.tv_sec * 1000000) + start_time.tv_usec;
    long end_time_micro = (end_time.tv_sec * 1000000) + end_time.tv_usec;
    long diff = end_time_micro - start_time_micro;
    printf("Read & Write calls: %d\nIt took %ld microseconds\n", count, diff);

    //closes the files
    close(decrypt_file_source_name);
    close(decrypt_file_destination);
    return 0;
}

int main(int argc, char** argv)
{

    char output_file_name[128];    //You may assume that the length of the output file name will not exceed 128 characters.

    //Checks to see that there are exactly 3 arguments
    if(argc != 3){
        write(1, ERROR, sizeof(ERROR));
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

    //Determines what mode to run the program with
    if(strcmp("-e", argv[1]) == 0){
        strcat(output_file_name, "_enc.txt");
        return file_encrypt(argv[2], output_file_name);
    }else if(strcmp("-d", argv[1]) == 0){
        strcat(output_file_name, "_dec.txt");
        return file_decrypt(argv[2], output_file_name);
    }else{
        write(1, ERROR, sizeof(ERROR));
        return -1;
    }

    return 0;

}
