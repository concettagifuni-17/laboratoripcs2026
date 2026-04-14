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
    bool is_sorted(const std::vector<T>& vec){
        int s = vec.size();
        for (int i = 1; i < s; i++){
            if (vec[i] < vec[i-1]){
                return false;
            }
        }
        return true;
    }