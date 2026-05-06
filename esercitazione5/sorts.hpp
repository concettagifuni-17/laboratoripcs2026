#pragma once

#include <algorithm>
#include <vector>

template<typename T> 
    void bubble_sort(std::vector<T>& vec){
        bool ordinato = false;
        while (!ordinato) {
            ordinato = true;
            int s = vec.size();
            for (int i=0; i < s -1; i++) {
                if (vec[i] > vec[i+1]) {
                    std::swap(vec[i], vec[i+1]);
                    ordinato = false;
                }
            }
        }
    }

template<typename T> 
    void insertion_sort(std::vector<T>& vec){
        int s = vec.size();
        for (int j = 1; j < s; j++) {
            T key = vec[j];
            int i = j - 1;
            while (i >= 0 && vec[i] > key) {
                vec[i+1] = vec[i];
                i = i - 1;
            }
            vec[i+1] = key;
        }
    }

template<typename T> 
    void selection_sort(std::vector<T>& vec){
        int s = vec.size();
        for (int i = 0; i < s - 1; i++) {
            int min = i;
            for (int j = i + 1; j < s; j++) {
                if (vec[j] < vec[min]){
                    min = j;
                }
            }
            std::swap(vec[i], vec[min]);
        }
    }

template<typename T>
    void merge(std::vector<T>& vec, int p, int q, int r){
        int n1 = q - p + 1;
        int n2 = r - q;
        //int inf = 10e10;
        std::vector<T> L(n1);
        std::vector<T> R(n2);

        for (int i = 0; i < n1; i++) {
            L[i] = vec[p + i];
        }
        for (int j = 0; j < n2; j++) {
            R[j] = vec[q + j + 1];
        }
        //L[n1] = inf;
        //R[n2] = inf;
        int i = 0;
        int j = 0;
        for (int k = p; k <= r; k++){
            bool infinito_L = (i >= n1);
            bool infinito_R = (j >= n2);
            if (infinito_R){
                vec[k] = L[i];
                i = i + 1;
            }
            else if (infinito_L) {
                vec[k] = R[j];
                j = j + 1;
            }
            else if (L[i] <= R[j]) {
                vec[k] = L[i];
                i = i+1;
            }
            else {
                vec[k] = R[j];
                j = j+ 1;
            }
        }    
    }

template<typename T>
    void merge_sort(std::vector<T>& vec, int p, int r){
        if (p < r) {
            int q = (p + r)/2;
            merge_sort(vec, p, q);
            merge_sort(vec, q +1, r);
            merge(vec, p, q, r);
        }
    }

template<typename T>
    int partition(std::vector<T>& vec, int p, int r){
        T x = vec[r];
        int i = p - 1;
        for (int j = p; j < r; j++){
            if (vec[j] <= x){
                i = i +1;
                std::swap(vec[i], vec[j]);
            }
        }
        std::swap(vec[i+1], vec[r]);
        return i + 1;
    }

template<typename T> 
    void quick_sort(std::vector<T>& vec, int p, int r){
        if (p < r) {
            int q = partition(vec, p, r);
            quick_sort(vec, p, q-1);
            quick_sort(vec, q + 1, r);
        }
    }

template<typename T> 
    bool is_sorted(const std::vector<T>& vec){
        int s = vec.size();
        for (int i = 1; i < s; i++){
            if (vec[i] < vec[i-1]){
                return false;
            }
        }
        return true;
    }

template<typename T>
void nuovo_quick(std::vector<T>& vec, int p, int r, int soglia){
    int dimensione = vec.size();
    if (dimensione <= soglia) {
        insertion_sort(vec);
    }
    else {
        quick_sort(vec, p, r);
    }
}
