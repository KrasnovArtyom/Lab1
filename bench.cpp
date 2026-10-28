#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include <functional>
#include <iomanip>
#include <algorithm>
#include "algs.h"

using Algo = std::function<bool(int*, int, int*, int)>;

//// Генератор массивов (худший случай)
//void generate_arrays(int N, int M, std::vector<int>& a, std::vector<int>& b) {
//    a.resize(N);
//    b.resize(M);
//
//    // Заполняем: a - четные, b - нечетные
//    for (int i = 0; i < N; i++) a[i] = i * 2;
//    for (int i = 0; i < M; i++) b[i] = i * 2 + 1;
//
//    // ХИТРОСТЬ: Добавляем общий элемент в самый конец массива b.
//    // Теперь алгоритм ОБЯЗАН пройти весь массив до конца, чтобы найти его.
//    // Компилятор не сможет сказать "результат известен заранее".
//    b.push_back(a.back());
//
//    // Пересортировываем b, чтобы сохранить порядок
//    std::sort(b.begin(), b.end());
//}

//Вот это реально худший случай
void generate_arrays(int N, int M, std::vector<int>& a, std::vector<int>& b) {
    a.resize(N);
    b.resize(M);

    // Заполняем B: нечетные числа 1, 3, 5, ..., 2M-1
    for (int i = 0; i < M; i++) b[i] = i * 2 + 1;

    // Заполняем A: берем элементы из B, но сдвигаем их на +1 (чтобы сделать четными)
    // И берем их с шагом M/N, чтобы покрыть весь диапазон B.
    for (int i = 0; i < N; i++) {
        long long pos = (long long)i * (M - 1) / std::max(1, N - 1);
        a[i] = b[(int)pos] + 1;
    }

    // Сортировка (на всякий случай, хотя они уже отсортированы)
    std::sort(a.begin(), a.end());
}

// Функция замера времени
double measure_time(Algo fn, std::vector<int>& a, std::vector<int>& b, int M) {
    double best_time = 1e9;
    int repeats = 3;

    for (int r = 0; r < repeats; r++) {
        auto start = std::chrono::steady_clock::now();

        // Приводим bool к int. volatile запрещает компилятору выкидывать вызов.
        volatile int result = fn(a.data(), (int)a.size(), b.data(), (int)b.size());
        (void)result;

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;

        if (diff.count() < best_time) {
            best_time = diff.count();
        }
    }
    return best_time;
}

bool bench_test() {
    std::vector<std::pair<std::string, Algo>> algos = {
        {"algo1_two_pointers",  algorithm1},
        {"algo2_binary_search", algorithm2},
        {"algo3_exponential",   algorithm3},
        {"algo4_double_binary", algorithm4}
    };

    int N = 1000; // Фиксируем N
    // Сетка от 10^3 до 10^8
    std::vector<int> sizes_M = { 1000, 10000, 100000, 1000000, 10000000, 100000000 };

    std::ofstream csv("results_test1.csv");
    if (!csv.is_open()) return false;

    // Заголовок: M, затем для каждого алгоритма время и шаги
    csv << "M";
    for (int i = 0; i < (int)algos.size(); i++) {
        csv << "," << algos[i].first << "_time," << algos[i].first << "_steps";
    }
    csv << "\n";

    for (int s = 0; s < (int)sizes_M.size(); s++) {
        int M = sizes_M[s];
        std::vector<int> a, b;
        generate_arrays(N, M, a, b);

        csv << M;
        for (int i = 0; i < (int)algos.size(); i++) {
            Algo fn = algos[i].second;
            double t = measure_time(fn, a, b, M);

            // Достаем шаги (они сохранились в глобальных переменных)
            long long steps = 0;
            if (i == 0) steps = steps_alg1;
            if (i == 1) steps = steps_alg2;
            if (i == 2) steps = steps_alg3;
            if (i == 3) steps = steps_alg4;

            csv << "," << std::fixed << std::setprecision(9) << t
                << "," << steps;
        }
        csv << "\n";
        std::cout << "Test 1: M = " << M << " done.\n";
    }
    csv.close();
    return true;
}

bool bench_diagonal() {
    std::vector<std::pair<std::string, Algo>> algos = {
        {"algo1_two_pointers",  algorithm1},
        {"algo2_binary_search", algorithm2},
        {"algo3_exponential",   algorithm3},
        {"algo4_double_binary", algorithm4}
    };

    // Сетка от 10 до 10^8 (8 точек)
    std::vector<int> sizes = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

    std::ofstream csv("results_diagonal.csv");
    if (!csv.is_open()) {
        std::cerr << "Error: cannot open results_diagonal.csv\n";
        return false;
    }

    // Заголовок: N=M, затем для каждого алгоритма время и шаги
    csv << "N_M";
    for (int i = 0; i < (int)algos.size(); i++) {
        csv << "," << algos[i].first << "_time," << algos[i].first << "_steps";
    }
    csv << "\n";

    for (int s = 0; s < (int)sizes.size(); s++) {
        int N = sizes[s];
        int M = sizes[s]; // N = M

        std::vector<int> a, b;
        generate_arrays(N, M, a, b);

        std::cout << "Diagonal Test: N = M = " << N << " ... \n";
        
        csv << N;
        for (int i = 0; i < (int)algos.size(); i++) {
            Algo fn = algos[i].second;
            double t = measure_time(fn, a, b, M);
            
            long long steps = 0;
            if (i == 0) steps = steps_alg1;
            if (i == 1) steps = steps_alg2;
            if (i == 2) steps = steps_alg3;
            if (i == 3) steps = steps_alg4;

            csv << "," << std::fixed << std::setprecision(9) << t
                << "," << steps;
        }
        csv << "\n";
        std::cout << "  done.\n";
    }

    csv.close();
    std::cout << "Diagonal benchmark finished. Saved to results_diagonal.csv\n";
    return true;
}

// Генератор для Evil Test (A > B, общий элемент в конце B)
void generate_evil_arrays(int N, int M, std::vector<int>& a, std::vector<int>& b) {
    a.resize(N);
    b.resize(M);

    // B: нечетные числа 1, 3, 5, ..., 2M-1
    for (int i = 0; i < M; i++) b[i] = i * 2 + 1;

    // A: четные числа, начинающиеся ПОСЛЕ конца B
    long long start_A = (long long)M * 2 + 2;
    for (int i = 0; i < N; i++) a[i] = (int)(start_A + i * 2);

    // Делаем последний элемент A общим с последним элементом B
    a[N - 1] = b[M - 1];

    std::sort(a.begin(), a.end());
}

bool bench_evil() {
    std::vector<std::pair<std::string, Algo>> algos = {
        {"algo1_two_pointers",  algorithm1},
        {"algo2_binary_search", algorithm2},
        {"algo3_exponential",   algorithm3},
        {"algo4_double_binary", algorithm4}
    };

    int N = 1000000; // N фиксировано
    std::vector<int> sizes_M = { 1000000, 10000000, 100000000 };

    std::ofstream csv("results_evil.csv");
    if (!csv.is_open()) {
        std::cerr << "Error: cannot open results_evil.csv\n";
        return false;
    }

    csv << "M";
    for (int i = 0; i < (int)algos.size(); i++) {
        csv << "," << algos[i].first << "_time," << algos[i].first << "_steps";
    }
    csv << "\n";

    for (int s = 0; s < (int)sizes_M.size(); s++) {
        int M = sizes_M[s];
        std::vector<int> a, b;
        generate_evil_arrays(N, M, a, b); // Используем злую генерацию

        std::cout << "Evil Test: M = " << M << " ... \n";

        csv << M;
        for (int i = 0; i < (int)algos.size(); i++) {
            Algo fn = algos[i].second;
            double t = measure_time(fn, a, b, M);

            long long steps = 0;
            if (i == 0) steps = steps_alg1;
            if (i == 1) steps = steps_alg2;
            if (i == 2) steps = steps_alg3;
            if (i == 3) steps = steps_alg4;

            csv << "," << std::fixed << std::setprecision(9) << t
                << "," << steps;
        }
        csv << "\n";
        std::cout << "  done.\n";
    }

    csv.close();
    std::cout << "Evil benchmark finished. Saved to results_evil.csv\n";
    return true;
}