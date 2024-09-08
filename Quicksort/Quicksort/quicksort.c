#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_ROWS 1062
#define MAX_COLS 150

#include <pthread.h>

#define NUM_THREADS 2

// Function prototypes
void *quicksort_thread(void *arg);
void *mergesort_thread(void *arg);
void *linear_sort_thread(void *arg);

char moto_data[MAX_ROWS][MAX_COLS]; // 2D matrix to store data

// Linear sort function to sort the entries in moto_data
void linear_sort(int n) {
    int i, j;
    char temp[MAX_COLS];

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (strcmp(moto_data[i], moto_data[j]) > 0) {
                strcpy(temp, moto_data[i]);
                strcpy(moto_data[i], moto_data[j]);
                strcpy(moto_data[j], temp);
            }
        }
    }
}

// Quicksort function to sort the entries in moto_data
void quicksort(int left, int right) {
    if (left >= right) {
        return;
    }

    char pivot[MAX_COLS];
    strcpy(pivot, moto_data[left]);

    int i = left + 1;
    int j = right;

    while (i <= j) {
        while (i <= j && strcmp(moto_data[i], pivot) < 0) {
            i++;
        }

        while (i <= j && strcmp(moto_data[j], pivot) > 0) {
            j--;
        }

        if (i <= j) {
            char temp[MAX_COLS];
            strcpy(temp, moto_data[i]);
            strcpy(moto_data[i], moto_data[j]);
            strcpy(moto_data[j], temp);
            i++;
            j--;
        }
    }

    char temp[MAX_COLS];
    strcpy(temp, moto_data[left]);
    strcpy(moto_data[left], moto_data[j]);
    strcpy(moto_data[j], temp);

    quicksort(left, j - 1);
    quicksort(j + 1, right);
}

// Merge function to merge two sorted arrays
void merge(char arr[MAX_ROWS][MAX_COLS], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    char L[n1][MAX_COLS], R[n2][MAX_COLS];

    for (i = 0; i < n1; i++) {
        strcpy(L[i], arr[left + i]);
    }

    for (j = 0; j < n2; j++) {
        strcpy(R[j], arr[mid + 1 + j]);
    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (strcmp(L[i], R[j]) <= 0) {
            strcpy(arr[k], L[i]);
            i++;
        } else {
            strcpy(arr[k], R[j]);
            j++;
        }
        k++;
    }

    while (i < n1) {
        strcpy(arr[k], L[i]);
        i++;
        k++;
    }

    while (j < n2) {
        strcpy(arr[k], R[j]);
        j++;
        k++;
    }
}

// Mergesort function to sort the entries in moto_data
void mergesort(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergesort(left, mid);
        mergesort(mid + 1, right);

        merge(moto_data, left, mid, right);
    }
}

// Function to create a subset of a matrix
void create_subset(char larger_matrix[][MAX_COLS], int num_rows, char subset[][MAX_COLS]) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            subset[i][j] = larger_matrix[i][j];
        }
    }
}

// Search function
void search(char moto_data[MAX_ROWS][MAX_COLS], int row) {
    // Sort the entries in moto_data using linear_sort
    clock_t start = clock();
    linear_sort(row);
    clock_t end = clock();
    double cpu_time_used_linear = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Sort the entries in moto_data using quicksort
    start = clock();
    quicksort(0, row - 1);
    end = clock();
    double cpu_time_used_quicksort = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Sort the entries in moto_data using mergesort
    start = clock();
    mergesort(0, row - 1);
    end = clock();
    double cpu_time_used_mergesort = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Print the sorted entries row by row for linear_sort
    printf("\nSorted Entries using Linear Sort:\n");
    for (int i = 0; i < row; i++) {
        //printf("Row %d: %s\n", i + 1, moto_data[i]);
    }
    printf("Time taken for Linear Sort: %f seconds\n", cpu_time_used_linear);

    // Print the sorted entries row by row for quicksort
    printf("\nSorted Entries using Quicksort:\n");
    for (int i = 0; i < row; i++) {
        //printf("Row %d: %s\n", i + 1, moto_data[i]);
    }
    printf("Time taken for Quicksort: %f seconds\n", cpu_time_used_quicksort);

    // Print the sorted entries row by row for mergesort
    printf("\nSorted Entries using Mergesort:\n");
    for (int i = 0; i < row; i++) {
        //printf("Row %d: %s\n", i + 1, moto_data[i]);
    }
    printf("Time taken for Mergesort: %f seconds\n", cpu_time_used_mergesort);

    // Example search for an entry in moto_data
    char search_entry[MAX_COLS] = "Royal Enfield";
    int found = 0;
    for (int i = 0; i < row; i++) {
        if (strstr(moto_data[i], search_entry) != NULL) {
            printf("\nFound entry at Row %d: %s\n", i + 1, moto_data[i]);
            found = 1;
            break;
        }
    }

    if (!found) {
        //printf("\nEntry not found.\n");
    }
}

// Main function
int main() {
    // File path
    FILE* fp = fopen("used_motorcycles.csv", "r");

    if (!fp)
        printf("Can't open file\n");
    else {
        char buffer[MAX_COLS];

        // Read the first line (column names)
        fgets(buffer, sizeof(buffer), fp);
        //printf("Column Names:\n");
        //printf("%s", buffer);

        int row = 0;
        // Read and save the first 10 rows
        while (row < MAX_ROWS && fgets(buffer, sizeof(buffer), fp)) {
            // Remove the newline character at the end
            buffer[strcspn(buffer, "\n")] = '\0';

            // Copy the entry to moto_data
            strcpy(moto_data[row], buffer);
            row++;
        }

        fclose(fp);

        // Create smaller matrices
        char medium[500][MAX_COLS]; // 2D matrix to store data
        char small[250][MAX_COLS]; // 2D matrix to store data
        char smaller[125][MAX_COLS]; // 2D matrix to store data

        // Populate the smaller matrices
        create_subset(moto_data, 500, medium);
        create_subset(moto_data, 250, small);
        create_subset(moto_data, 125, smaller);

        // Call the search function
        printf("\nLarge matrix: \n");
        search(moto_data, row); // Pass the row variable as an argument

        //calls the search function on the 3 smaller matrices
        printf("\nMedium matrix: \n");
        search(medium, 500);
        printf("\nSmall matrix: \n");
        search(small, 250);
        printf("\nSmaller matrix: \n");
        search(smaller, 125);

        //use a parellel approach to compare all times

        // Create thread IDs and attributes
        //pthread_t threads[NUM_THREADS];
        //pthread_attr_t attr;
        //pthread_attr_init(&attr);
        //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        // Launch threads for quicksort and mergesort
        //pthread_create(&threads[0], &attr, quicksort_thread, (void *) medium);
        //pthread_create(&threads[1], &attr, mergesort_thread, (void *) small);

        // Wait for threads to complete
        //for (int i = 0; i < NUM_THREADS; i++) {
            //pthread_join(threads[i], NULL);
        //}

        // Call linear sort on the smaller matrix
        //linear_sort_thread((void *) smaller);

    }

    return 0;
}

// Quick sort thread function
void *quicksort_thread(void *arg) {
    char (*matrix)[MAX_COLS] = (char (*)[MAX_COLS]) arg;
    quicksort(0, 499);
    pthread_exit(NULL);
}

// Merge sort thread function
void *mergesort_thread(void *arg) {
    char (*matrix)[MAX_COLS] = (char (*)[MAX_COLS]) arg;
    mergesort(0, 249);
    pthread_exit(NULL);
}

// Linear sort thread function
void *linear_sort_thread(void *arg) {
    char (*matrix)[MAX_COLS] = (char (*)[MAX_COLS]) arg;
    linear_sort(124);
    pthread_exit(NULL);
}