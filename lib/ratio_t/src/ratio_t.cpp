#include <numeric>
#include <cmath>

#include "ratio_t.h"

ratio_t::ratio_t(long long num) :
    ratio_t(num, 1ll)
{
}

ratio_t::ratio_t(double num)
{
    m_denominator = s_fromDoubleDenominator;
    m_numerator = static_cast<long long>(num * static_cast<double>(s_fromDoubleDenominator));
    reduce();
}

ratio_t::ratio_t(long long numerator, long long denominator) :
    m_numerator{ numerator }, m_denominator{ denominator }
{
    reduce();
}

ratio_t& ratio_t::reduce()
{
    if (m_numerator == 0 && m_denominator == 0)
    {
        return *this;
    }
    if (m_numerator == 0)
    {
        m_denominator = 1;
        return *this;
    }
    if (m_denominator == 0)
    {
        m_numerator = m_numerator < 0 ? -1 : 1;
        return *this;
    }
    if (m_denominator < 0)
    {
        m_denominator *= -1;
        m_numerator *= -1;
    }

    long long reduceBy = std::gcd(m_numerator, m_denominator);
    m_numerator /= reduceBy;
    m_denominator /= reduceBy;

    return *this;
}

ratio_t& ratio_t::operator+=(const ratio_t& ratio)
{
    if (m_numerator == 0 && m_denominator == 0)
    {
        return *this;
    }
    if (ratio.m_numerator == 0 && ratio.m_denominator == 0)
    {
        m_numerator = 0;
        m_denominator = 0;
        return *this;
    }
    if (std::abs(m_numerator) == 1 && m_denominator == 0)
    {
        if (ratio.m_numerator == -m_numerator && ratio.m_denominator == 0)
        {
            m_numerator = 0;
            m_denominator = 1;
        }
        return *this;
    }

    long long common_denominator = std::lcm(m_denominator, ratio.m_denominator);
    m_numerator = (
        (common_denominator / m_denominator) * m_numerator
        + (common_denominator / ratio.m_denominator) * ratio.m_numerator
    );
    m_denominator = common_denominator;
    return this->reduce();
}

ratio_t& ratio_t::operator-=(const ratio_t& ratio)
{
    return *this += -ratio;
}

ratio_t& ratio_t::operator*=(const ratio_t& ratio)
{
    m_numerator *= ratio.m_numerator;
    m_denominator *= ratio.m_denominator;
    return this->reduce();
}

ratio_t& ratio_t::operator/=(const ratio_t& ratio)
{
    if (std::abs(m_numerator) == 1 && m_denominator == 0)
    {
        if (std::abs(ratio.m_numerator) == 1 && ratio.m_denominator == 0)
        {
            m_numerator = m_numerator * ratio.m_numerator;
            m_denominator = 1;
            return *this;
        }
    }

    ratio_t reverseRatio = ratio;
    std::swap(reverseRatio.m_numerator, reverseRatio.m_denominator);
    return *this *= reverseRatio;
}

ratio_t ratio_t::operator+() const
{
    return *this;
}

ratio_t ratio_t::operator-() const
{
    ratio_t newRatio = *this;
    newRatio.m_numerator *= -1;
    return newRatio;
}

std::strong_ordering ratio_t::operator<=>(const ratio_t& ratio) const
{
    if (m_denominator == 0 && ratio.m_denominator == 0)
    {
        if (m_numerator == 0)
        {
            if (ratio.m_numerator == 0)
            {
                return std::strong_ordering::equal;
            }
            if (ratio.m_numerator == -1)
            {
                return std::strong_ordering::greater;
            }
            return std::strong_ordering::less;
        }
        if (m_numerator == 1)
        {
            return ratio.m_numerator == 1
                ? std::strong_ordering::equal : std::strong_ordering::greater;
        }
        return ratio.m_numerator == -1
                ? std::strong_ordering::equal : std::strong_ordering::less;
    }
    if (m_denominator == 0)
    {
        return m_numerator == 0 || m_numerator == -1
            ? std::strong_ordering::less : std::strong_ordering::greater;
    }
    if (ratio.m_denominator == 0)
    {
        return ratio.m_numerator == 0 || ratio.m_numerator == -1
            ? std::strong_ordering::greater : std::strong_ordering::less;
    }

    return (m_numerator * ratio.m_denominator) <=> (ratio.m_numerator * m_denominator);
}

bool ratio_t::operator==(const ratio_t& ratio) const
{
    return m_numerator == ratio.m_numerator && m_denominator == ratio.m_denominator;
}

ratio_t operator+(ratio_t firstRatio, const ratio_t& secondRatio)
{
    return firstRatio += secondRatio;
}

ratio_t operator-(ratio_t firstRatio, const ratio_t& secondRatio)
{
    return firstRatio -= secondRatio;
}

ratio_t operator*(ratio_t firstRatio, const ratio_t& secondRatio)
{
    return firstRatio *= secondRatio;
}

ratio_t operator/(ratio_t firstRatio, const ratio_t& secondRatio)
{
    return firstRatio /= secondRatio;
}

ratio_t::operator double() const
{
    return static_cast<double>(m_numerator) / static_cast<double>(m_denominator);
}

std::ostream& operator<<(std::ostream& os, const ratio_t& ratio)
{
    if (ratio.m_denominator == 0)
    {
        if (ratio.m_numerator == 0)
        {
            return os << "NaN";
        }
        if (ratio.m_numerator < 0)
        {
            os << '-';
        }
        return os << "Inf";
    }
    return os << ratio.m_numerator << "/" << ratio.m_denominator;
}

std::istream& operator>>(std::istream& is, ratio_t& ratio)
{
    std::string ratioStr{};
    is >> ratioStr;

    if (ratioStr[0] == '+')
    {
        ratioStr.erase(0);
    }

    if (ratioStr == "NaN")
    {
        ratio.m_numerator = 0;
        ratio.m_denominator = 0;
        return is;
    }
    if (ratioStr == "Inf")
    {
        ratio.m_numerator = 1;
        ratio.m_denominator = 0;
        return is;
    }
    if (ratioStr == "-Inf")
    {
        ratio.m_numerator = -1;
        ratio.m_denominator = 0;
        return is;
    }

    ratio.m_numerator = 0;
    ratio.m_denominator = 0;

    bool isDenominator = false;
    bool isMinus = false;
    for (size_t i = 0; i < ratioStr.size(); i++)
    {
        if (ratioStr[i] == '-')
        {
            isMinus = !isMinus;
            continue;
        }
        if (ratioStr[i] == '+')
        {
            continue;
        }
        if (ratioStr[i] == '/')
        {
            isDenominator = true;
            continue;
        }

        if (isDenominator)
        {
            ratio.m_denominator *= 10;
            ratio.m_denominator += ratioStr[i] - '0';
        }
        else
        {
            ratio.m_numerator *= 10;
            ratio.m_numerator += ratioStr[i] - '0';
        }
    }
    if (isMinus)
    {
        ratio.m_numerator *= -1;
    }
    if (!isDenominator)
    {
        ratio.m_denominator = 1;
    }

    ratio.reduce();

    return is;
}

ratio_t& ratio_t::pow(long long power)
{
    if (m_numerator == 0 && m_denominator == 0)
    {
        return *this;
    }

    ratio_t powRatio = *this;
    if (power < 0)
    {
        power *= -1;
        powRatio = ratio_t(1ll) / powRatio;
    }

    m_numerator = 1;
    m_denominator = 1;
    for (long long currPow = 1; power != 0; powRatio *= powRatio, currPow <<= 1)
    {
        if (power & currPow)
        {
            *this *= powRatio;
            power ^= currPow;
        }
    }
    return *this;
}

ratio_t& ratio_t::ceil()
{
    if (m_denominator == 0 || m_numerator % m_denominator == 0)
    {
        return *this;
    }
    m_numerator += m_denominator - m_numerator % m_denominator;
    return this->reduce();
}

ratio_t& ratio_t::floor()
{
    if (m_denominator == 0 || m_numerator % m_denominator == 0)
    {
        return *this;
    }
    m_numerator -= m_numerator % m_denominator;
    return this->reduce();
}

ratio_t& ratio_t::round()
{
    return (*this += ratio_t(1, 2)).floor();
}