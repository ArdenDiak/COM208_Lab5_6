/* Functions to process and return statistics about a file */

#include "../include/csapp.h"
#include "../include/connex.h"
#include "../include/fileStats.h"

#define MAX_UNIQUE_WORDS 1000
#define MAX_WORD_LENGTH  50

int binSearch(char **list, int N, char *target)
{
    if(N == 0) return -1;

    int hi= N;
    int lo= 0;
    int mid= (hi-lo)/2;

    while(lo < hi){
        mid= lo + (hi-lo)/2;
        if(strcmp(list[mid], target) == 0)
            return mid;
        else if(strcmp(list[mid], target) < 0)
            lo= mid + 1;
        else
            hi= mid;  //not `hi = mid-1` (check loop invariants)
    }
        /* no need for these conditions either, check proof
        if(strcmp(list[hi], target) != 0 && hi == (lo+1)){
            return -1;
        }
        */
    return -1;
}

void quickSort(char **A, int lo, int hi)
//@requires 0 <= lo <= hi <= len(A)
//@ensures _is_sorted(A);
{
    if(hi-lo <= 1) return;
    int pi= lo + (hi-lo)/2;

    int mid= partition(A, lo, pi, hi);
    quickSort(A, lo, mid);
    quickSort(A, mid+1, hi);
    return;
}

int partition(char **A, int lo, int pi, int hi)
//@requires 0 <= lo <= pi
//@requires pi < hi <= len(A)
//@requires 0 <= \result < hi
//@requires ge_seg(A[\result], A, lo, \result);
//@requires le_seg(A[\result], A, \result+1, hi);
{
    char *pivot= A[pi];
    swap(A, lo, pi);

    int left= lo+1;
    int right= hi;
    while(left < right)
    //@loop_invariant lo+1 <= left && left <= right && right <= hi;
    //@loop_invariant ge_seg(pivot, A, lo+1, left);
    //@loop_invariant le_seg(pivot, A, right, hi);
    {
        //if(A[left] < pivot)
        if(strcmp(A[left], pivot) < 0){
            left++;
        }else{
            //@assert A[left] > pivot
            swap(A, left, right-1);
            right--;
        }
    }
    //@assert left == right
    swap(A, lo, left-1);
    return left-1;
}

void swap(char **A, int x, int y)
{
    int word_len= MAX_WORD_LENGTH;
    char tmp[word_len];

    strcpy(tmp, A[x]);
    strcpy(A[x], A[y]);
    strcpy(A[y], tmp);
}

void getStats(char *line, fileStats *stats){
    int w_idx;
    stats->length = (int)(strlen(line) - 1);

    char* token = strtok(line, " ");
    while (token != NULL) {
        //make token lowercase
        int i;
        while(token[i]){
            token[i]= tolower(token[i]);
            i++;
        }

        if((w_idx = binSearch(stats->unique_w, stats->numUniqueW, token)) < 0){
            //add new word, sort list, incr. occ. count
            int N= stats->numUniqueW;
            strcpy(stats->unique_w[N], token);
            stats->numUniqueW++;
            quickSort(stats->unique_w, 0, stats->numUniqueW);

            stats->unique_w_occ[w_idx]= 1;
        }else{ //ele found in list
            stats->unique_w_occ[w_idx]++;
        }

        token = strtok(NULL, " ");
    }
}

void fileStatsInit(fileStats *stats){
    stats->unique_w = malloc(sizeof(char*) * MAX_UNIQUE_WORDS);
    stats->unique_w_occ = malloc(sizeof(int) * MAX_UNIQUE_WORDS);
    stats->length = 0;
    stats->numUniqueW = 0;
}

void fileStatsClean(fileStats *stats){
    free(stats->unique_w);
    free(stats->unique_w_occ);
}
