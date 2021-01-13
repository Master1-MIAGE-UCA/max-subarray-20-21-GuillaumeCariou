#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

struct tablo {
  int * tab;
  int size;
};

void printArray(struct tablo * tmp) {
  int size = tmp->size;
  printf("---- Array of size %i ---- \n", size);
  for (int i = 0; i < size; ++i) {
    printf("%i ", tmp->tab[i]);
  }
  printf("\n");
}

void printResult(struct tablo * tmp, int Max, int begin, int end) {
  printf("%d ", Max);
  for (int i = begin; i <= end; ++i) {
    printf("%i ", tmp->tab[i]);
  }
  printf("\n");
}

struct tablo * allocateTablo(int size) {
  struct tablo * tmp = malloc(sizeof(struct tablo));
  tmp->size = size;
  tmp->tab = malloc(size*sizeof(int));
  return tmp;
}

struct tablo * reverseArray(struct tablo * source){
  struct tablo * copy = malloc(sizeof(struct tablo));
  copy->tab = malloc(source->size*sizeof(int));
  copy->size = source->size;
  #pragma omp parallel for
  for (int i = 0; i < source->size; i++){
    copy->tab[source->size-1-i] = source->tab[i];
  }
  return copy;
}



struct tablo * sum_prefix(struct tablo source) {//=========================================================================================OK
  //montee
  struct tablo * a = malloc(sizeof(struct tablo));
  a->tab = malloc(source.size*2*sizeof(int));
  a->size =source.size*2;
  a->tab[0] = 0;

  #pragma omp parallel for
	for (int i = source.size-1; 0 <= i; i--)
  {
    a->tab[i+source.size] = source.tab[i];
  }

  int m = log(source.size)/log(2);

  for (int i = m -1; 0 <= i; i--){
    int inf = pow(2,i);
    int sup = pow(2, i + 1) - 1;

    #pragma omp parallel for
    for (int j = inf; j <= sup; j++){
      a->tab[j] = a->tab[j*2] + a->tab[(j*2)+1];
    }
  }

  //descente
  struct tablo * b = malloc(sizeof(struct tablo));
  b->tab= malloc(source.size*2*sizeof(int));
  b->size=source.size*2;
  b->tab[0] = 0;


  m = log(a->size/2)/log(2);
  b->tab[1] = 0;

  for (int i = 1; i <= m; i++){
    int inf = pow(2,i);
    int sup = pow(2, i + 1) - 1;

    #pragma omp parallel for
    for (int j = inf; j <= sup; j++){
      if(j%2 == 0){//fils gauche
        b->tab[j] = b->tab[j/2];
      }else{//fils droit
        b->tab[j] = b->tab[j/2] + a->tab[j-1];
      }
    }
  }

  //final
  struct tablo * final = malloc(sizeof(struct tablo));
  final->size = a->size/2;
  final->tab = malloc(final->size*sizeof(int));

  #pragma omp parallel for
  for (int i = 0; i < final->size; i++){
    final->tab[i] = a->tab[i+final->size] + b->tab[i+final->size];
  }
  
  b->size = final->size;
  b->tab = final->tab;

  return final;
}



struct tablo * sum_suffix(struct tablo source) {//=========================================================================================OK
  struct tablo * copy = reverseArray(&source);
  copy = sum_prefix(*copy);
  copy = reverseArray(copy);
  return copy;
}



struct tablo * max_prefix(struct tablo source) {//=========================================================================================OK
  //montee
  struct tablo * a = malloc(sizeof(struct tablo));
  a->tab = malloc(source.size*2*sizeof(int));
  a->size =source.size*2;
  a->tab[0] = 0;

  #pragma omp parallel for
	for (int i = source.size-1; 0 <= i; i--)
  {
    a->tab[i+source.size] = source.tab[i];
  }

  int m = log(source.size)/log(2);

  for (int i = m - 1; 0 <= i; i--){
    int inf = pow(2,i);
    int sup = pow(2, i + 1) - 1;

    #pragma omp parallel for
    for (int j = inf; j <= sup; j++){
      a->tab[j] = fmax(a->tab[j*2], a->tab[(j*2)+1]);
    }
  }

  //descente
  struct tablo * b = malloc(sizeof(struct tablo));
  b->tab = malloc(source.size*2*sizeof(int));
  b->size=source.size*2;
  b->tab[0] = 0;


  m = log(a->size/2)/log(2);
  b->tab[1] = 0;

  for (int i = 1; i <= m; i++){
    int inf = pow(2,i);
    int sup = pow(2, i + 1) - 1;

    #pragma omp parallel for
    for (int j = inf; j <= sup; j++){
      if(j%2 == 0){//fils gauche
        b->tab[j] = b->tab[j/2];
      }else{//fils droit
        b->tab[j] = fmax(b->tab[j/2], a->tab[j-1]);
      }
    }
  }

  //final
  struct tablo * final = malloc(sizeof(struct tablo));
  final->size = a->size/2;
  final->tab = malloc(final->size*sizeof(int));

  #pragma omp parallel for
  for (int i = 0; i < final->size; i++){
    final->tab[i] = fmax(a->tab[i+final->size], b->tab[i+final->size]);
  }
  
  b->size = final->size;
  b->tab = final->tab;

  return final;
}



struct tablo * max_suffix(struct tablo source) {//=========================================================================================OK
  struct tablo * copy = reverseArray(&source);
  copy = max_prefix(*copy);
  copy = reverseArray(copy);
  return copy;
}



//https://cboard.cprogramming.com/c-programming/4073-string-integer-array.html
struct tablo * foo (char *line) {
  int num, i = 0, len;
  int buff[128];//taile de nombre maximal
  while ( sscanf( line, "%d%n", &num, &len) == 1 ) {
    buff[i] = num;
    line += len;
    i++;
  }
  struct tablo * array = allocateTablo(i);
  #pragma omp parallel for
  for (int j = 0; j < i; j++)
  {
    array->tab[j] = buff[j];
  }
  return array;
}

int main(int argc, char **argv) {
  FILE *f;
  if(argc == 2){
    f = fopen(argv[1], "r");
  }else{
    return 1;
  }
  
  int MAXCHAR = 1000;
  char line[MAXCHAR];
  fgets(line,MAXCHAR,f);
  struct tablo * Q = foo(line);

  struct tablo * PSUM = sum_prefix(*Q);
  struct tablo * SSUM = sum_suffix(*Q);
  struct tablo * SMAX = max_suffix(*PSUM);
  struct tablo * PMAX = max_prefix(*SSUM);

  //étape 5
  struct tablo * M = allocateTablo(Q->size);
  int Ms;
  int Mp;
  #pragma omp parallel for
  for (int i = 0; i < Q->size; i++)
  {
    Ms = PMAX->tab[i] - SSUM->tab[i] + Q->tab[i];
    Mp = SMAX->tab[i] - PSUM->tab[i] + Q->tab[i];
    M->tab[i] = Ms + Mp - Q->tab[i];
  }

  //etape 6
  int max = M->tab[0];
  int begin = 0;
  int end = 0;
  for (int i = 1; i < M->size; i++)
  {
    if(M->tab[i] > max){
      max = M->tab[i];
      begin = i;
    }else if(M->tab[i] == max){
      end = i;
    }
  }
  printResult(Q,max,begin,end);
  return 0;
}
