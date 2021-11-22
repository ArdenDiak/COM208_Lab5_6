
typedef struct {
  int length;
  int numUniqueW;
  char **unique_w;
  int *unique_w_occ;
} fileStats;

/*Searching and Sorting*/
int binSearch(char **list, int N, char *target);
void quickSort(char **A, int lo, int hi);
int partition(char **A, int lo, int pi, int hi);
void swap(char **A, int x, int y);

void getStats(char *line, fileStats *stats);
void fileStatsInit(fileStats *stats);
void fileStatsClean(fileStats *stats);
