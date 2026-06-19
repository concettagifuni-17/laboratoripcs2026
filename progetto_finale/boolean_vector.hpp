#pragma once

#include <iostream>
#include <vector>


// Vettori booleani di lunghezza m, dove m è il numero degli archi.
// Ogni posizione del vettore rappresenta un arco.
// Un arco "attivo" è segnato da 1, altrimenti da 0.
template<typename T>
using BoolVector = std::vector<T>;


// ============================================================
// Prodotto scalare modulo 2
//
// Formula:
// <a,b> = somma_i a[i] * b[i] mod 2
//
// Darà:
// 0 se il numero di 1 nelle stesse posizioni è pari
// 1 se il numero di 1 nelle stesse posizioni è dispari
// ============================================================

template<typename T>
int scalar_product_mod2(
    const BoolVector<T>& a,
    const BoolVector<T>& b
) {
    int risultato = 0;

    for (std::size_t i = 0; i < a.size(); i++) {
        risultato = (risultato + a[i] * b[i]) % 2;
    }

    return risultato;
}


// ============================================================
// Differenza simmetrica tra vettori booleani
//
// È uno XOR componente per componente.
//
// 0 XOR 0 = 0
// 0 XOR 1 = 1
// 1 XOR 0 = 1
// 1 XOR 1 = 0
// ============================================================

template<typename T>
BoolVector<T> xor_vector(
    const BoolVector<T>& a,
    const BoolVector<T>& b
) {
    BoolVector<T> risultato(a.size(), 0);

    for (std::size_t i = 0; i < a.size(); i++) {
        risultato[i] = (a[i] + b[i]) % 2;
    }

    return risultato;
}


// ============================================================
// Stampa del vettore booleano
//
// Serve per controllare a schermo i vettori S_i e i cicli
// rappresentati come vettori di incidenza.
// ============================================================

template<typename T>
void print_bool_vector(const BoolVector<T>& v) {
    std::cout << "[";

    for (std::size_t i = 0; i < v.size(); i++) {
        std::cout << v[i];

        if (i + 1 < v.size()) {
            std::cout << " ";
        }
    }

    std::cout << "]";
}


// ============================================================
// Contatore di 1 in un vettore booleano
//
// In un vettore di incidenza corrisponde al numero di archi
// usati dal ciclo.
// ============================================================

template<typename T>
int count_ones(const BoolVector<T>& v) {
    int contatore = 0;

    for (std::size_t i = 0; i < v.size(); i++) {
        if (v[i] == 1) {
            contatore = contatore + 1;
        }
    }

    return contatore;
}