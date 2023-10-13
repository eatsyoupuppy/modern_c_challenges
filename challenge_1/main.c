#include <stddef.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define RED(string) "\x1b[31m" string "\x1b[0m"

#define MAX_INPUT 255

enum Mode {merge_sort_mode = 1, quick_sort_mode = 2 };


size_t create_array(const char* file_path, double** list) {
    FILE* file;

    file = fopen(file_path, "r");

    if (!file) {
        printf("Error opening file ");
        printf("" RED("%s") " \n", file_path);
        exit(EXIT_FAILURE);
    }

    size_t mem_size = sizeof(double) * 100000;
    *list = (double*)malloc(mem_size);
    if (*list == NULL) {
        exit(EXIT_FAILURE);
    }

    size_t size = 0;
    double number = 0.0;
    while (fscanf(file, "%lf", &number) != EOF) {
        if (!(size % 100000)) {
        mem_size += sizeof(double) * 100000;
        *list = (double*)realloc(*list, mem_size);
        if (*list == NULL) {
            exit(EXIT_FAILURE);
        } 
        }

        (*list)[size] = number;
        size++;
    }

    fclose(file);
    return size;
}


bool is_sorted(double** list, size_t size) {
    for (size_t i = 0; i < size-1; ++i) {
        if ( (*list)[i] > (*list)[i+1] ) {
        return false;
        }
    }
    return true;
}


void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}


size_t partition(double* list, size_t low, size_t high) {
    double pivot = list[low];
    size_t i = low;
    size_t j = high;

    while(true) {
        while (list[i] < pivot) i++;
        while (list[j] > pivot) j--;

        if (i >= j) return j;

        swap(&list[i], &list[j]);
    }
}


void quick_sort(double* list, size_t low, size_t high) {
    if (low < high) {
        size_t part = partition(list, low, high);

        quick_sort(list, low, part);
        quick_sort(list, part + 1, high);
    }
}


void merge(double* list, const size_t list_size, double* left, size_t left_size, double* right, size_t right_size) {
    size_t i = 0; size_t l = 0; size_t r = 0;

    while (l < left_size && r < right_size) {
        if (left[l] < right[r]) {
            list[i] = left[l];
            i++;
            l++;
        } else {
            list[i] = right[r];
            i++;
            r++;
        }
    }

    while(l < left_size) {
			list[i] = left[l];
			i++;
			l++;
		}

		while(r < right_size) {
			list[i] = right[r];
			i++;
			r++;
		}

    free(left);
    free(right);
}


void merge_sort(double* list, size_t size, const size_t list_size) {
	if (size <= 1) return;

    size_t middle = size / 2;

    double* left = (double*)malloc(sizeof(double) * middle);
    double* right = (double*)malloc(sizeof(double) * (size - middle));

    size_t j = 0;
    size_t i = 0;

    for (; i < size; ++i) {
        if(i < middle) {
            left[i] = list[i];
        } else {
            right[j] = list[i];
            j++;
        }
    }

    merge_sort(left, middle, list_size);
    merge_sort(right, size - middle, list_size);
    merge(list, list_size, left, middle, right, size - middle);
}


int main (int argc, char* argv[argc+1]) {  
    char file_path[MAX_INPUT] = "\0";
    int mode = -1;
    int opt;
    while ((opt = getopt(argc, argv, ":-f:-m:-h")) != -1 ) {
        switch (opt) {
        case 'f':
            strcpy(file_path, optarg);
            break;
        case 'm':
            sscanf(optarg, "%d", &mode);
            break;
        case ':':
            printf("Missing value\n");
            break;
        case 'h':
            printf("USAGE:\n");
            printf("\t%s [flags] [options]\n", argv[0]);

            printf("\nFLAGS:\n");
            printf("\t-f \"file path\" or \"file name\"\n");
            printf("\t-m <MODE>\n");
            printf("\t-h to show this help text\n");

            printf("\nMODES\n");
            printf("\tmerge sort = 1\n");
            printf("\tquick sort = 2\n");

            printf("\nEXAMPLE:\n");
            printf("\t%s -f list.txt -m 1\n\n",argv[0]);

            return EXIT_SUCCESS;
        case '?':
            printf("Couldn't parse command line\n\n");
            break;
        }
    }

    for(; optind < argc; optind++) {      
        printf("Argument not recognized: %s\n", argv[optind]);  
    } 

    if( (mode < merge_sort_mode) || (mode > quick_sort_mode)  || (file_path[0] == '\0') || (argc != 5) ) {
        printf("Missing required arguments\n");
        printf("Use -h for help\n");
        return EXIT_FAILURE;
    }

    double *list = NULL;
    size_t size = create_array(file_path, &list);
    const size_t list_size = size;
    usleep(500000);

    printf("Sorting list " RED("%s") " using ", file_path);

    switch (mode) {
    case merge_sort_mode:
        printf(ANSI_COLOR_RED "merge sort mode\n" ANSI_COLOR_RESET);

        merge_sort(list, size, list_size);
        break;
        case quick_sort_mode:
        printf(ANSI_COLOR_RED "quick sort mode\n" ANSI_COLOR_RESET);
        quick_sort(list, 0, size-1);
        break;
    }

    usleep(500000);
    if (is_sorted(&list, size) ) printf(ANSI_COLOR_GREEN "List was sorted successfully\n" ANSI_COLOR_RESET);
    else printf( ANSI_COLOR_RED"List was not sorted\n" ANSI_COLOR_RESET);

    free(list);
    return EXIT_SUCCESS;
}


