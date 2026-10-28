#include <iostream>
#include <vector>
#include <algorithm>
#include "algs.h"

//void test_algo3_sparse() {
//    int N = 1000;
//    int M = 1000000000;
//
//    std::vector<int> a(N), b(M);
//
//    // Заполняем b: 0, 2, 4, 6... (четные)
//    for (int i = 0; i < M; i++) b[i] = i * 2;
//
//    // Внутри генератора:
//    int step = M / N; // 10^9 / 10^3 = 10^6
//    for (int i = 0; i < N; i++) {
//        a[i] = b[i * step] + 1;
//    }
//
//    std::cout << "Testing algo3 on sparse data...\n";
//    bool res = algorithm3(a.data(), N, b.data(), M);
//
//    std::cout << "M: " << M << std::endl;
//    std::cout << "Result: " << (res ? "true" : "false") << "\n";
//    std::cout << "Steps: " << steps_alg3 << "\n";
//    // Ожидание: steps должно быть значительно больше, чем на плотных данных (2998),
//    // так как алгоритму придется делать большие прыжки.
//}

int main() {
    //correctness_test();
    //random_test();
    bench_test(); //Худший случай
    //bench_diagonal(); //Увеличиваем и N и M
    //bench_evil(); //Худший случай для двух указателей, а то чё они такие быстрые
    return 0;
}