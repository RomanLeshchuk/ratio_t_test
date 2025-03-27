#ifndef RATIO_T_H
#define RATIO_T_H

#include <ostream>
#include <istream>

class ratio_t
{
public:
    ratio_t() = default;

    ratio_t(const ratio_t& ratio) = default;
    ratio_t& operator=(const ratio_t& ratio) = default;

    ratio_t(ratio_t&& ratio) = default;
    ratio_t& operator=(ratio_t&& ratio) = default;

    explicit ratio_t(long long num);
    explicit ratio_t(double num);
    ratio_t(long long numerator, long long denominator);

    ratio_t& operator+=(const ratio_t& ratio);
    ratio_t& operator-=(const ratio_t& ratio);
    ratio_t& operator*=(const ratio_t& ratio);
    ratio_t& operator/=(const ratio_t& ratio);

    ratio_t operator+() const;
    ratio_t operator-() const;

    std::partial_ordering operator<=>(const ratio_t& ratio) const;
    std::partial_ordering operator<=>(long long num) const;

    bool operator==(const ratio_t& ratio) const;
    bool operator==(long long num) const;

    explicit operator double() const;

    friend std::ostream& operator<<(std::ostream& os, const ratio_t& ratio);
    friend std::istream& operator>>(std::istream& is, ratio_t& ratio);

    ratio_t& pow(long long power);
    ratio_t& ceil();
    ratio_t& floor();
    ratio_t& round();

    bool isPlusInf() const;
    bool isMinusInf() const;
    bool isInf() const;
    bool isNaN() const;

    ~ratio_t() = default;

private:
    ratio_t& reduce();

    long long m_numerator = 0;
    long long m_denominator = 1;

    static constexpr long long s_fromDoubleDenominator = static_cast<long long>(1e9);
};

ratio_t operator+(ratio_t firstRatio, const ratio_t& secondRatio);
ratio_t operator-(ratio_t firstRatio, const ratio_t& secondRatio);
ratio_t operator*(ratio_t firstRatio, const ratio_t& secondRatio);
ratio_t operator/(ratio_t firstRatio, const ratio_t& secondRatio);

std::ostream& operator<<(std::ostream& os, const ratio_t& ratio);
std::istream& operator>>(std::istream& is, ratio_t& ratio);

#endif