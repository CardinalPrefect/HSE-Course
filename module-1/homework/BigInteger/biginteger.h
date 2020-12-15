#include <iostream>
#include <vector>
#include <string>

class BigInteger {
public:
    BigInteger();

    BigInteger(int num);

    BigInteger(const BigInteger& other);

    BigInteger(BigInteger&& other) noexcept;

    BigInteger& operator=(const BigInteger& other);

    BigInteger& operator=(BigInteger&& other) noexcept;

    BigInteger operator+(const BigInteger& other) const;

    BigInteger operator-(const BigInteger& other) const;

    BigInteger& operator+=(const BigInteger& other);

    BigInteger& operator-=(const BigInteger& other);

    BigInteger operator*(int a) const;

    BigInteger operator*(const BigInteger& other) const;

    BigInteger operator/(const BigInteger& other) const;

    BigInteger operator%(const BigInteger& other) const;

    BigInteger& operator*=(const BigInteger& other);

    BigInteger& operator/=(const BigInteger& other);

    BigInteger& operator%=(const BigInteger& other);

    BigInteger operator-() const;

    BigInteger& operator++();

    BigInteger operator++(int);

    BigInteger& operator--();

    BigInteger operator--(int);

    operator bool() const;

    std::string toString() const;

    ~BigInteger();

    friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);

    friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);

    friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);

    friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);

    friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);

    friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);

    friend std::ostream& operator<<(std::ostream& out, const BigInteger& obj);

    friend std::istream& operator>> (std::istream& in, BigInteger& obj);

private:
    BigInteger(std::vector<unsigned char>& data, bool negative = false);

    BigInteger shift(size_t step);

private:
    std::vector<unsigned char> data;
    const unsigned char base = 100;
    bool is_negative = false;
};
