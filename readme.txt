void printResult(...) : 		non parallel
struct tablo * read_file (...) :	non parallel
struct tablo * allocateTablo(...) : 	non parallel
struct tablo * reverseArray(...) :	parallel


struct tablo * sum_prefix(...) : 	parallel
struct tablo * sum_suffix(...) : 	parallel
struct tablo * max_prefix(...) : 	parallel
struct tablo * max_suffix(...) : 	parallel
etape 5 (in main):			parallel
etape 6 (in main): 			non parallel







gcc -g -Wall -std=c99 GuillaumeCariou.c -lm -fopenmp
valgrind --leak-check=full ./a.out testme
