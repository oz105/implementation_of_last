#include <stdio.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>  
#include <utmp.h>


void printDetails(struct utmp *arr, int num_lines, int count){
    for (size_t i = 0; (i < num_lines && i < count); i++)
    {
        if ((arr[i].ut_type) == BOOT_TIME) // if so is system boot
        {
            printf("%s\t ", arr[i].ut_user);
            printf("system boot \t ");
            printf("%s\t", arr[i].ut_host);
            printf("\n");
        }
        else
        {
            printf("%s\t ", arr[i].ut_user);
            printf("%s\t\t", arr[i].ut_line);
            printf(" %s\t\t\t", arr[i].ut_host);
            printf("\n");
        }
    }
}

int main (int argc, char* argv[]){
    int print_all = 0;
    int num_lines;
    if(argc == 2){ // if dont enter num of lines // should we keep it?
        num_lines = atoi(argv[1]);
        if(num_lines <= 0){
            printf("ERROR: The command 'slast' must be followed with a non-nagative number of lines\n");
            exit(1);
        }
    }
    else{
        print_all = 1 ;
    }
    int count = 0 ;

    struct utmp current;
    int utmp_check;
    int structerSize = sizeof(struct utmp);
    utmp_check = open(WTMP_FILE, O_RDONLY);

    if(utmp_check < 0){ // cant open the file
        perror("File not open"); 
        exit(1);
    }
    // printf("the utmp is  : %d \n ",utmp_check);

    while (read(utmp_check, &current, structerSize) == structerSize){
        if (current.ut_type != 8 && current.ut_type == 1){// means or dead process or The system's runlevel and shouldnt count.
            count ++ ;
        }
    }
    // printf ("the count is %d \n",count );

    close(utmp_check);

    struct utmp arr[count];
    int counter = count - 1;
    utmp_check = open(WTMP_FILE, O_RDONLY);
    
    if(utmp_check < 0 ){ // cant open the file
        perror("File not open"); 
        exit(1);
    }

    while (read(utmp_check, &current, structerSize) == structerSize){
        if (current.ut_type != 8 && current.ut_type != 1){
            arr[counter--] = current;
        }
    }
    if(print_all == 1){
        printDetails(arr,count,count);
    }

    else{
        printDetails(arr,num_lines,count);
    }

    close(utmp_check);

    return 0;

}

/**

helpfull links:
how to print the info in columns-
demo2s.com/c/c-printf-print-columns-of-data.html

https://www.tutorialspoint.com/c_standard_library/c_function_ctime.htm

https://linux.die.net/man/5/wtmp 

https://www.toolbox.com/tech/operating-systems/question/reading-var-adm-wtmp-101907/

https://www.geeksforgeeks.org/c-program-find-size-file/

fopen vs open 
	https://stackoverflow.com/questions/1658476/c-fopen-vs-open

how to read wtmp file
    https://stackoverflow.com/questions/10324144/how-to-read-wtmp-file
use open system call
    https://stackoverflow.com/questions/26333279/reading-the-linux-utmp-file-without-using-fopen-and-fread

 */

// 
