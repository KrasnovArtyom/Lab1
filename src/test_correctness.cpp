#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include "algs.h" 

using Algo = std::function<bool(int*, int, int*, int)>;

// Структура для одного тестового случая
struct Case {
    std::string name;    // Название теста (для вывода)
    std::vector<int> a;  // Первый массив (меньший)
    std::vector<int> b;  // Второй массив (больший или равный)
    bool expected;       // Ожидаемый результат (true/false)
};

// Вспомогательная функция для запуска алгоритма
bool run(Algo fn, std::vector<int>& a, std::vector<int>& b) {
    return fn(a.data(), (int)a.size(), b.data(), (int)b.size());
}

bool correctness_test() {
    // Список алгоритмов
    std::vector<std::pair<std::string, Algo>> algos = {
        {"algorithm1 (two pointers)",  algorithm1},
        {"algorithm2 (binary search)", algorithm2},
        {"algorithm3 (exponential)",   algorithm3},
        {"algorithm4 (double binary)", algorithm4}
    };

    // Список тестовых случаев (N <= M)
    std::vector<Case> cases = {
        // 1.
        {"example_true",        {1, 2, 3},         {3, 4, 5},         true},
        {"example_false",       {2, 4, 6},         {3, 5, 7},         false},

        // 2. Пустые массивы
        {"both_empty",          {},                {},                false},
        {"a_empty",             {},                {1, 2, 3},         false},
        {"b_empty",             {},                {},                false}, // N<=M: если b пуст, то и a пуст

        // 3. Один элемент
        {"single_equal",        {5},               {5},               true},
        {"single_less",         {1},               {2},               false},
        {"single_in_many",      {3},               {1, 2, 3, 4, 5},   true},
        {"single_not_in_many",  {8},               {1, 2, 3, 4, 5},   false},

        // 4. Одинаковые по размеру
        {"identical",           {1, 2, 3, 4, 5},   {1, 2, 3, 4, 5},   true},
        {"disjoint",            {1, 2, 3},         {4, 5, 6},         false},

        // 5. Общий элемент в разных местах
        {"common_at_start",     {1, 2, 3},         {1, 4, 5},         true},
        {"common_at_end",       {1, 2, 3},         {3, 4, 5},         true},
        {"common_middle",       {1, 2, 9},         {3, 9, 10},        true},

        // 6. Сильно разные длины (N << M)
        {"tiny_in_huge",        {3},               {1, 2, 3, 4, 5, 6, 7}, true},
        {"tiny_not_in_huge",    {8},               {1, 2, 3, 4, 5, 6, 7}, false},

        // 7. Отрицательные числа
        {"negatives_true",      {-5, -3, -1},      {-4, -3, -2},      true},
        {"negatives_false",     {-5, -3, -1},      {-4, -2, 0},       false},

        // 8. Дубликаты
        {"dup_in_a",            {1, 1, 2},         {2, 3, 4},         true},
        {"dup_in_both",         {1, 2, 2},         {2, 2, 3},         true},
        {"dup_no_common",       {1, 1, 1},         {2, 2, 2},         false}
    };

    int fails = 0;
    int total = 0;

    // Внешний цикл: идем по массиву алгоритмов
    for (int i = 0; i < (int)algos.size(); i++) {

        // Достаем пару (имя, функция) по индексу i
        const std::string& aname = algos[i].first;
        Algo fn = algos[i].second;

        // Внутренний цикл: идем по массиву тестовых случаев
        for (int j = 0; j < (int)cases.size(); j++) {
            ++total;
            // Берем текущий тестовый случай
            Case& c = cases[j];

            // Вызываем алгоритм
            bool got = fn(c.a.data(), (int)c.a.size(), c.b.data(), (int)c.b.size());

            // Проверяем
            if (got != c.expected) {
                std::cerr << "FAIL [" << aname << "] " << c.name
                    << ": expected=" << (c.expected ? "true" : "false")
                    << " got=" << (got ? "true" : "false") << "\n";
                ++fails;
            }
        }
    }

    // Итоговый вывод
    if (fails == 0) {
        std::cout << "OK: 0 failures / " << total << " checks\n";
    }
    else {
        std::cout << "FAILED: " << fails << " failures / " << total << " checks\n";
    }

    // Возвращаем 0, если всё ок, и 1, если были ошибки
    return fails ? false : true;
}