#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//  * together with type modifier
char* name;
const char* const path_name;

//  continued declarations not allowed
unsigned const*const a, b;

//  if pointer can't be NULL, [] is used instead of *
size_t strlen(char const string[static 1]);
int main(int argc, char* argv[argc+1]);
//  same as
size_t strlen(const char *string);
int main(int argc, char **argv);

//  define variables close to their first use
int some_number = 0;

//  example formatting
int main(int argc, char* argv[argc+1]) {
    if (argc > 1) {
        while (true) {
            printf("Hi");
        }
    } else {
        do {
            printf("hello");
        } while (false);
    }
    return EXIT_SUCCESS;
}

void example(void handler (void) ) {
    //  example if else alternative
    if (true) printf("Hi");
    else printf("Hello");

    //  example for loop
    for (size_t i = 0; i < 10; ++i) {
        printf("Something");
    }
    for (size_t i = 0; i < 10; ++i) printf("Something else");

    //example while and do while
    while (true) printf("Something");
    do printf("Something else"); while (true);

}
