#include "algs.h"

long long steps_alg1 = 0;
long long steps_alg2 = 0;
long long steps_alg3 = 0;
long long steps_alg4 = 0;

//Get [l, r)
int bin_search(int* a, int l, int r, int key, long long& counter) {
    if (l >= r)
        return l;
    int mid;
    r -= 1;
    while (l <= r) {
        counter++;
        mid = (l + r) / 2;
        if (a[mid] == key)
            return mid;
        if (a[mid] < key)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return l;
}

//Again get [l,r)
int exp_search(int* a, int l, int r, int key,long long& counter) {
    if (l >= r)
        return l;
    int step = 1;
    if (a[l] >= key)
        return l;
    while (l + step < r && a[l + step] < key) {
        counter++;
        step *= 2;
    }
    return bin_search(a, l + step / 2, (l + step >= r) ? r : l + step + 1, key, counter);
}

//Two pointers
//O(N+M)
bool algorithm1(int* n, int N, int* m, int M) {
    steps_alg1 = 0;
    // Обеспечиваем N <= M
    if (N > M) { std::swap(n, m); std::swap(N, M); }
    int i = 0;
    int j = 0;
    while (i < N && j < M) {
        steps_alg1++;
        if (n[i] == m[j])
            return true;
        if (n[i] < m[j])
            i++;
        else
            j++;
    }
    return false;
}

//O(Nlog(M))
bool algorithm2(int* n, int N, int* m, int M) {
    steps_alg2 = 0;
    // Обеспечиваем N <= M
    if (N > M) { std::swap(n, m); std::swap(N, M); }
    int prev_try = 0;
    for (int i = 0; i < N; i++) {
        prev_try = bin_search(m, prev_try, M, n[i], steps_alg2);
        if (prev_try >= M)
            return false;
        if (n[i] == m[prev_try])
            return true;
    }
    return false;
}

//O(Nlog(M))
bool algorithm3(int* n, int N, int* m, int M) {
    steps_alg3 = 0;
    // Обеспечиваем N <= M
    if (N > M) { std::swap(n, m); std::swap(N, M); }
    int prev_try = 0;
    for (int i = 0; i < N; i++) {
        prev_try = exp_search(m, prev_try, M, n[i], steps_alg3);
        if (prev_try >= M)
            return false;
        if (n[i] == m[prev_try])
            return true;
    }
    return false;
}

//Get [lo_n, hi_n) [lo_m, hi_m)
bool rec(int* n, int lo_n, int hi_n,
    int* m, int lo_m, int hi_m) {
    steps_alg4++;
    if (lo_n >= hi_n || lo_m >= hi_m) return false;
    int mid = lo_n + (hi_n - lo_n) / 2;
    int pos = bin_search(m, lo_m, hi_m, n[mid], steps_alg4);
    //if (pos >= hi_m) return rec(n, lo_n, mid, m, mid + 1, hi_m); 
    if (pos < hi_m && m[pos] == n[mid]) return true; //пооверка pos < hi_m нужна, чтобы проверить выход за границу массива m
    return rec(n, lo_n, mid, m, lo_m, pos) || rec(n, mid + 1, hi_n, m, pos, hi_m);
}
//logN*logM?
bool algorithm4(int* n, int N, int* m, int M) {
    steps_alg4 = 0;
    // Обеспечиваем N <= M
    if (N > M) { std::swap(n, m); std::swap(N, M); }
    return rec(n, 0, N, m, 0, M);
}

//bool algorithm5(int* n, int N, int* m, int M) {
//    int prev_try = 0;
//    int i = 0, j = 0;
//    while (i < N && j < M) {
//        if (n[i] == m[j])
//            return true;
//        if (n[i] > m[j])
//            j = bin_search(m, j + 1, M, n[i]);
//        else
//            i = bin_search(n, i + 1, N, m[j]);
//    }
//    return false;
//}