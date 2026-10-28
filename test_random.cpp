#include <iostream>
#include <vector>
#include <random>
#include <set>
#include <algorithm>
#include <functional>
#include "algs.h"

using Algo = std::function<bool(int*, int, int*, int)>;

// Эталонная (медленная, но 100% правильная) функция
bool brute(const std::vector<int>& a, const std::vector<int>& b) {
    std::set<int> s(b.begin(), b.end()); // Кидаем второй массив в множество
    for (int x : a) {
        if (s.count(x)) return true;     // Если элемент из первого есть во втором — успех
    }
    return false;
}

bool random_test() {
    std::vector<std::pair<std::string, Algo>> algos = {
        {"algorithm1 (two pointers)",  algorithm1},
        {"algorithm2 (binary search)", algorithm2},
        {"algorithm3 (exponential)",   algorithm3},
        {"algorithm4 (double binary)", algorithm4}
    };

    // Настройка генератора случайных чисел
    std::mt19937 rng(12345); // 12345 — это seed (зерно). Одинаковый seed = одинаковые тесты при каждом запуске.
    std::uniform_int_distribution<int> len_d(0, 50);   // Длина массивов от 0 до 50
    std::uniform_int_distribution<int> val_d(-50, 50); // Значения элементов от -50 до 50

    int fails = 0;
    const int ITER = 5000; // Количество случайных пар

    for (int it = 0; it < ITER; ++it) {
        // 1. Генерируем случайные длины
        int na = len_d(rng);
        int nb = len_d(rng);

        // 2. Создаем массивы и заполняем случайными числами
        std::vector<int> a(na), b(nb);
        for (int& x : a) x = val_d(rng);
        for (int& x : b) x = val_d(rng);

        // 3. Сортируем и удаляем дубликаты (чтобы соответствовало условию задачи)
        std::sort(a.begin(), a.end());
        a.erase(std::unique(a.begin(), a.end()), a.end());
        std::sort(b.begin(), b.end());
        b.erase(std::unique(b.begin(), b.end()), b.end());

        // 4. Обеспечиваем N <= M (первый массив меньше или равен второму)
        if (a.size() > b.size()) {
            std::swap(a, b);
        }

        // 5. Считаем правильный ответ
        bool expected = brute(a, b);

        // 6. Проверяем все алгоритмы
        for (int i = 0; i < (int)algos.size(); i++) {
            const std::string& aname = algos[i].first;
            Algo fn = algos[i].second;

            bool got = fn(a.data(), (int)a.size(), b.data(), (int)b.size());

            if (got != expected) {
                // Если нашли баг — выводим контрпример!
                std::cerr << "FAIL [" << aname << "] iteration=" << it << "\n";
                std::cerr << "  a = {";
                for (int x : a) std::cerr << x << " ";
                std::cerr << "}\n  b = {";
                for (int x : b) std::cerr << x << " ";
                std::cerr << "}\n  expected=" << (expected ? "true" : "false")
                    << " got=" << (got ? "true" : "false") << "\n";
                ++fails;
                if (fails > 5) return false; // Хватит, если ошибок слишком много
            }
        }
    }

    if (fails == 0) {
        std::cout << "OK: 0 failures / " << ITER << " random cases\n";
        return true;
    }
    else {
        std::cout << "FAILED: " << fails << " failures\n";
        return false;
    }
}