#ifndef BIG_H
#define BIG_H

#include "includes/conversion.h"
#include <bits/stdc++.h>
#include <iostream>

// typedef uint32_t small;
typedef uint64_t large;
typedef int64_t slarge;

const int SZ = 60;  // size of array
const int POW = 31; // max value for base

#define MAX (large(1U) << POW) - 1U
#define RANGE(i, n) for (int i = 0; i < (int)n; i++)
#define RANGEr(i, n) for (int i = n; i >= 0; i--)
#define LO(x) (large) x &MAX
#define HI(x) (large)(x >> POW)

class big
{
public:
    small l = 1;
    small *data = (small *)malloc(sizeof(small) * SZ);
    char sign = 0; // zero is positive
    //unsigned char start = 0;
    big(string s)
    {
        fill(data, data + SZ, 0);
        if (s[0] == '-')
        {
            sign = 1;
            s = s.substr(1, s.length());
            l = convert(s, data, POW);
            return;
        }
        l = convert(s, data, POW); // case remaining for binary input
    }
    big() { fill(data, data + SZ, 0); }
    big(big &n)
    {
        l = n.l;
        sign = n.sign;
        RANGE(i, SZ)
        data[i] = n.data[i];
    }
    big(small n)
    {
        fill(data, data + SZ, 0);
        data[0] = n;
    }
    void zeros()
    {
        RANGEr(i, l) { data[i] = 0; }
    }

    ~big() { free(data); }

    void operator+=(big &t);
    void operator+=(int t);
    void operator-=(big &t);
    void operator<<=(int l);
    void operator>>=(int l);
    void operator=(big a);
};

void print(big *b, bool i);

big *blockShiftLeft(big *a, int t);
big *blockShiftRight(big *a, int t);

int compare(big *a, big *b);
bool operator>(big &a, big &b);
bool operator<(big &a, big &b);
inline bool operator==(big &a, big &b);
big *addHelp(big *a, big *b);
big *subHelp(big *a, big *b);
void mulSingle(big *z, big *x, small t, int start);
big *mulHelp(big *a, big *b);
inline big *signs(big *a, big *b);
big *divide(big *a, big *b);
big *arithmetic(big *a, big *b, char i);

inline big &operator+(big &a, big &b) { return *arithmetic(&a, &b, 1); }

inline big &operator-(big &a, big &b) { return *arithmetic(&a, &b, -1); }

inline big &operator*(big &a, big &b)
{
    if (a.l > b.l)
        return *mulHelp(&a, &b);
    return *mulHelp(&b, &a);
}

inline big &operator/(big &a, big &b) { return *divide(&a, &b); }

#endif