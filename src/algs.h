#pragma once
#include <algorithm>

// Глобальные счетчики шагов
extern long long steps_alg1;
extern long long steps_alg2;
extern long long steps_alg3;
extern long long steps_alg4;

//Алгоритмы
int exp_search(int* a, int l, int r, int key);
int bin_search(int* a, int l, int e, int key);
bool algorithm1(int* n, int N, int* m, int M);
bool algorithm2(int* n, int N, int* m, int M);
bool algorithm3(int* n, int N, int* m, int M);
bool algorithm4(int* n, int N, int* m, int M);

//Корректность+время
bool correctness_test();
bool random_test();
bool bench_test();
bool bench_diagonal();
bool bench_evil();
