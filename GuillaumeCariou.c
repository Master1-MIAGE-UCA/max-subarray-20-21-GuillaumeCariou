#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

struct tablo {
  int * tab;
  int size;
};

void printArray(struct tablo * tmp) {
  printf("---- Array of size %i ---- \n", tmp->size);
  int size = tmp->size;
  int i;
  for (i = 0; i < size; ++i) {
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

void montee(struct tablo * source, struct tablo * destination) {
  #pragma omp parallel for
	for (int i = source->size-1; 0 <= i; i--)
  {
    destination->tab[i+source->size] = source->tab[i];
  }

  int m = log(source->size)/log(2);

  for (int i = m -1; 0 <= i; i--)
  {
    int inf = pow(2,i);
    int sup = pow(2, i + 1) - 1;

    #pragma omp parallel for
    for (int j = inf; j <= sup; j++)
    {
      destination->tab[j] = destination->tab[j*2] + destination->tab[(j*2)+1];
    }
    
  }
}

void descente(struct tablo * a, struct tablo * b) {
  int m = log(a->size/2)/log(2);
  b->tab[1] = 0;

  for (int i = 1; i <= m; i++)
  {
    int inf = pow(2,i);
    int sup = pow(2, i + 1) - 1;

    #pragma omp parallel for
    for (int j = inf; j <= sup; j++)
    {
      if(j%2 == 0){//fils gauche
        b->tab[j] = b->tab[j/2];
      }else{//fils droit
        b->tab[j] = b->tab[j/2] + a->tab[j-1];
      }
    }
  }
  
}

void final(struct tablo * a, struct tablo *b) {
  struct tablo final;
  final.size = a->size/2;
  final.tab = malloc(final.size*sizeof(int));

  #pragma omp parallel for
  for (int i = 0; i < final.size; i++)
  {
    final.tab[i] = a->tab[i+final.size] + b->tab[i+final.size];
  }
  

  b->size = final.size;
  b->tab = final.tab;
}

void generateArray(struct tablo * s) {
  //construction d'un tableau pour tester
  s->size=8;
  s->tab=malloc(s->size*sizeof(int));
  s->tab[0]=3;
  s->tab[1]=1;
  s->tab[2]=7;
  s->tab[3]=0;
  s->tab[4]=4;
  s->tab[5]=1;
  s->tab[6]=6;
  s->tab[7]=3;

}

int main(int argc, char **argv) {
  struct tablo source;

  generateArray(&source);
  printArray(&source);

  struct tablo * a = malloc(sizeof(struct tablo));
  a->tab = malloc(source.size*2*sizeof(int));
  a->size =source.size*2;
  a->tab[0] = 0;
  montee(&source, a);
  printArray(a);

  struct tablo * b = malloc(sizeof(struct tablo));
  b->tab= malloc(source.size*2*sizeof(int));
  b->size=source.size*2;
  b->tab[0] = 0;
  descente(a, b);
  printArray(b);
	
  final(a,b);
  printArray(b);
}
