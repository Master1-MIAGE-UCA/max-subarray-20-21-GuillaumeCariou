x---- Array of size 8 ----
3 1 7 0 4 1 6 3
---- Array of size 16 ----
0 25 11 14 4 7 5 9 3 1 7 0 4 1 6 3
---- Array of size 16 ----
0 0 0 11 0 4 11 16 0 3 4 11 11 15 16 22
---- Array of size 8 ----
3 4 11 11 15 16 22 25

Sortie de prefix_sum
PSUM OK
SSUM
SMAX
PMAX

gcc -g -Wall -std=c99 GuillaumeCariou.c -lm -fopenmp
valgrind --leak-check=full ./a.out testme
