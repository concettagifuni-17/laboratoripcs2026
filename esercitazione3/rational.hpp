#pragma once
#include <iostream>
#include <concepts>

template<typename I> requires std::integral<I>
class rational {
    I num_;
    I den_;

    void simplify() {
        if (den_ == 0) {
            if (num_ > 0) {
                num_ = 1;    // trasforma ogni razionale del tipo (intero finito)/0 nella forma 'semplificata' 1/0 che indica +inf
            }
            else if (num_ < 0) {
                num_ = -1; } // trasforma ogni razionale del tipo - (intero finito)/0 nella forma 'semplificata' -1/0 che indica -inf
            else {
            num_ = 0; }   //  0/0 indica NaN
            return;
        }
        if (den_ < 0) {
            num_ = -num_;  //'semplificazione' dei segni
            den_ = -den_;
        }

        if (num_ == 0 && den_!= 0) {
            den_ = 1;  // gestione caso 0/(intero finito), lo trasfroma sempre in 0/1 (che poi stampo semplicemente come zero)
            return;
        }
        I a = num_;
        I b = den_;
        if (a < 0) {a = -a;}
        if (b < 0) {b = -b;}

        I MCD = 1;  //calcolo MCD per semplificare i razionali finiti
        for (I i = 2; i<= a && i <= b; i++) {
            if (a % i == 0 && b % i == 0) {
                MCD = i;
            }
        }
        num_ = num_/MCD;
        den_ = den_/MCD;
    }
    
public:
    rational() 
        : num_(I{0}), den_(I{1}) 
        {}
    
    rational(const I& pnum, const I& pden)
        : num_(pnum), den_(pden)
        { simplify();}
    
    I num() const { return num_; }
    I den() const { return den_; }
    
    rational& operator+=(const rational& other) {
    // qualsiasi somma con NaN da NaN
    if ((den_ == 0 && num_ == 0) || (other.den_ == 0 && other.num_ == 0)) {
        num_ = 0;
        den_ = 0;
        return *this;
    }
    // +Inf + -Inf  oppure -Inf + +Inf, forme indeterminate che danno NaN
    if (den_ == 0 && other.den_ == 0) {
        if ((num_ > 0 && other.num_ < 0) || (num_ < 0 && other.num_ > 0)) {
            num_ = 0;
            den_ = 0;
            return *this;
        }
    }
    // se il primo dei due è infinito
    if (den_ == 0) {
        return *this;
    }
    // se il secondo è infinito
    if (other.den_ == 0) {
        num_ = other.num_; // così consideriamo il segno dell'infinito 
        den_ = 0;
        return *this;
    }
    // caso normale
    num_ = num_ * other.den_ + den_ * other.num_;
    den_ = den_ * other.den_;
    simplify();
    return *this;
    }

    rational operator+(const rational& other) const {
        rational ret = *this;
        ret += other;
        return ret;}

    rational& operator-=(const rational& other) {
    // qualsiasi sottrazione con NaN da NaN
    if ((den_ == 0 && num_ == 0) || (other.den_ == 0 && other.num_ == 0)) {
        num_ = 0;
        den_ = 0;
        return *this;
    }
    // +Inf - +Inf   oppure -Inf - -Inf, forma indeterminata che da NaN
    if (den_ == 0 && other.den_ == 0) {
        if ((num_ > 0 && other.num_ > 0) || (num_ < 0 && other.num_ < 0)) {
            num_ = 0;
            den_ = 0;
            return *this;}
        }
    // se uno dei due è infinito
    if (den_ == 0) {
        return *this;
    }
    if (other.den_ == 0) {
        if (other.num_ > 0) {
            num_ = -1;
            den_ = 0;
        } else {
            num_ = 1;
            den_ = 0;
        }
        return *this;
    }

    // caso normale
    num_ = num_ * other.den_ - den_ * other.num_;
    den_ = den_ * other.den_;
    simplify();
    return *this;
    }
    
    rational operator-(const rational& other) const {
        rational ret = *this;
        ret -= other;
        return ret;
    }
        
    rational& operator*=(const rational& other) {
    // qualsiasi cosa moltiplicata per NaN da Nan
    if ((den_ == 0 && num_ == 0) || (other.den_ == 0 && other.num_ == 0)) {
        num_ = 0;
        den_ = 0;
        return *this;
    }
    // 0 * Inf oppure Inf * 0 forma indeterminata da NaN
    if ((den_ == 0 && other.num_ == 0) || (other.den_ == 0 && num_ == 0)) {
        num_ = 0;
        den_ = 0;
        return *this;
    }
    // se uno dei due è infinito
    if (den_ == 0 || other.den_ == 0) {
        if ((num_ > 0 && other.num_ > 0) || (num_ < 0 && other.num_ < 0)) {
            num_ = 1;
            den_ = 0;} 
        else {
            num_ = -1;
            den_ = 0;
        }
    return *this;
    }
    // caso normale
    num_ = num_ * other.num_;
    den_ = den_ * other.den_;
    simplify();
    return *this;
    }
    
    rational operator*(const rational& other) const {
        rational ret = *this;
        ret *= other;
        return ret;
    }

    rational& operator/=(const rational& other) {
    // qualsiasi divisione con NaN da NaN
    if ((den_ == 0 && num_ == 0) || (other.den_ == 0 && other.num_ == 0)) {
        num_ = 0;
        den_ = 0;
        return *this;
    }
    // secondo razionale uguale a 0 (operazione indefinita) ottengo NaN
    if (other.den_ != 0 && other.num_ == 0) {
        num_ = 0;
        den_ = 0;
        return *this;
    }
    // 0 / infinito = 0
    if (den_ != 0 && num_ == 0 && other.den_ == 0) {
        num_ = 0;
        den_ = 1;
        return *this;
    }
    // inf / inf = NaN 
    if (den_ == 0 && other.den_ == 0) {
        num_ = 0;
        den_ = 0;
        return *this;
    }
    // inf / finito = inf 
    if (den_ == 0) {
        if ((num_ > 0 && other.num_ > 0) || (num_ < 0 && other.num_ < 0)) {
            num_ = 1;
            den_ = 0;} 
        else {
            num_ = -1;
            den_ = 0;
        }
        return *this;
    }
    // finito / inf = 0
    if (other.den_ == 0) {
        num_ = 0;
        den_ = 1;
        return *this;
    }
    // caso normale
    num_ = num_ * other.den_;
    den_ = den_ * other.num_;
    simplify();
    return *this;
    }
    rational operator/(const rational& other) const {
        rational ret = *this;
        ret /= other;
        return ret;
    }
};

template<typename I> std::ostream&
operator<<(std::ostream& os, const rational<I>& r) {
    if (r.den() == 0){
        if (r.num() == 1 ) {
        os << "Inf";
        }
        else if (r.num() == -1) {
            os << "-Inf";
        }
        else {
            os << "NaN";
        }
    }
    else if (r.num() == 0 && r.den() != 0) {
        os << "0"; 
    }
    else {
    os << r.num() << "/" << r.den();
    }  
    return os;
}