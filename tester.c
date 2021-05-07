/*
 * This file represents a sample tester to test your syscalls. You can
 * compile it with the following command: gcc -g -o tester tester.c
 *
 * You can modify it but do no modify the syscalls number.
 *
 * Important: Run always a tracee that does either a loop, a wait or a
 * sleep otherwise it will be difficult to test.
 * 
 * We will not consider complex processes that perform memory
 * allocation between the two syscalls.
 * 
 * In addition, we will always consider that severals instances of a 
 * tracee (with a same name) is always the same.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){

    // The name of the tracee that you want to test. In this example,
    // we launch a simple c/asm program, called tracee, that does an infinite loop.

    // Do not use "./" as prefix.
    char *name = NULL;
    long n = 0;

    if (argc < 2){
        fprintf(stderr, "%s is expecting one parameter (e.g., tracee)\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    name = argv[1];

    // 1. Get the number of pages (TOTAL #of pages) of a process or several process(es) with a given name.
    long nb_pages = syscall(385, name, strlen(name));
    if (nb_pages <= 0){
        fprintf(stderr, "[ERROR] The given process(es) do(es) not have any pages or do(es) not exist\n");
        exit(n);
    }

    // IMPORTANT: for simplicity we will always consider that the bitmap is initialized with 0.
    char *bitmap = calloc(nb_pages, sizeof(char));
    if (!bitmap){
        fprintf(stderr, "[ERROR] Allocation failure\n");
        exit(EXIT_FAILURE);
    }

    // 2. Get the bitmap initialized by the kernel to have only read-only and present pages.
    // Note that a page with the 'r-xp' permission is also a read-only page.
    n = syscall(386, bitmap, name , strlen(name));
    if (n < 0){
        free(bitmap);
        fprintf(stderr, "[ERROR] Impossible to initialize the bitmap structure in the kernel\n");
        exit(n);
    }

    // Iterating to see only the useful pages
    for(size_t i = 0; i < nb_pages; i++){
        if (bitmap[i] == 1){
            printf("[%d]: %d\n", i, bitmap[i]);
        }
    }

    // Fly away...
    free(bitmap);

    return EXIT_SUCCESS;
}
