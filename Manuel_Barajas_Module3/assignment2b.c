#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main(void) {
  int i=0;
  int j = 0;
  char* string[1225];
  int ccount[1225];
  int alpha[26] = { 0 };
  char line[100];
  FILE *file; 
  FILE *results;
  file = fopen("male_census.rtf","r");
  results = fopen("results.txt","w");
  int isUnique;
  char c;
  int count = 0;
  int tid;
  while(fgets(line, sizeof line, file)!=NULL) {
    // printf("%s", line);
    string[i]=line;
    for(i = 0; i < 26; i++){
      alpha[i] = 0;
    }
    isUnique = 0;
#pragma omp parallel for
    for(i = 0; i < 10; i++){
      tid = omp_get_thread_num();
      printf("%d is now working on %s",tid,line);
      c = line[i];
      if('A' <= c && c<='Z'){
        if(alpha[c-'A'] == 1){
          continue;
        }
      }
      else{
        alpha[c-'A'] = 1;
        fprintf(results, "%c\n", c);
      }
      ccount
    }
    j++;
  }
  printf("the number of words with unique letters is = %d",count);
  fclose(file);
  fclose(results);
  return 0;
}
