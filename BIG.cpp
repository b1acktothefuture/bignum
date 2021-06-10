#include "includes/convert.h"
#include <bits/stdc++.h>
#include <iostream>

typedef uint32_t small;
typedef uint64_t large;
typedef int64_t slarge;

const int SZ = 40; // size of array
#define POW 16     // max value for base

#define MAX (large(1U) << POW) - 1U
#define RANGE(i, n) for (int i = 0; i < (int)n; i++)
#define RANGEr(i, n) for (int i = n; i >= 0; i--)
#define LO(x) (large) x &MAX
#define HI(x) (large)(x >> POW)

large reuse;
slarge sreuse;

class big;
inline big *signs(big *a, big *b);

class big
{
public:
    small l = 0;
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
        {
            data[i] = n.data[i];
        }
    }
    void zeros()
    {
        RANGE(i, SZ) { data[i] = 0; }
    }

    ~big() { free(data); }

    void operator+=(big &t);
    void operator+=(int t);
};

void big::operator+=(big &t)
{
    reuse = 0;
    l = max(l, t.l);
    RANGE(i, l)
    {
        reuse = t.data[i] + data[i] + HI(reuse);
        data[i] = LO(reuse);
    }
    if (HI(reuse))
        data[l++] = HI(reuse);
}

void big::operator+=(int t) // works iff t>=0
{
    reuse = 0;
    reuse = data[0] + t;
    data[0] = LO(reuse);
    if (!HI(reuse))
        return;
    int i = 1;
    while (1)
    {
        reuse = data[i] + HI(reuse);
        data[i] = LO(reuse);
        i++;
        if (!HI(reuse))
            break;
    }
}

void print(big *b)
{
    if (b->sign)
        cout << "-";
    RANGEr(i, b->l)
            cout
        << b->data[i] << " ";
    cout << " (base 2^" << POW << ")\n";
}

void decimal(big b)
{
    if (b.sign)
        cout << "-";
    unsigned long long sum = 0;
    RANGEr(i, b.l - 1)
        sum = (MAX + 1) * (sum) + b.data[i];
    cout << sum << "\n";
}

big *addHelp(big *a, big *b) // adds two postive numbers
{
    reuse = 0;
    big *c = new big();
    c->l = max(a->l, b->l);
    RANGE(i, c->l)
    {
        reuse = HI(reuse) + a->data[i] + b->data[i];
        c->data[i] = LO(reuse);
    }
    if (HI(reuse))
        c->data[c->l++] = HI(reuse);
    return c;
}

big *subHelp(big *a, big *b) // a- b // returns valid result if a>=b
{
    big *c = new big();
    reuse = (large)1 << POW;
    c->l = max(a->l, b->l);
    RANGE(j, c->l)
    {
        reuse = MAX + a->data[j] - b->data[j] + HI(reuse);
        c->data[j] = LO(reuse);
    }
    c->sign = (int)HI(reuse) - 1;
    return c;
}

void mulSingle(big *z, big *x, small t, int start)
{
    z->zeros();
    z->l = start + x->l;
    reuse = 0;
    RANGE(i, x->l)
    {
        reuse = large(x->data[i]) * t + HI(reuse);
        z->data[start + i] = LO(reuse);
    }
    if (HI(reuse))
        z->data[z->l++] = HI(reuse);
}

// big *karatsuba(big *a, big *b, int a1, int a2, int b1, int b2)
// {
//     int l1 = max(a->l, b->l);
//     int l2 = l1 / 2;
//     if (a1 == a2 && b1 == b2)
//     {
//         big *z = new big();
//         reuse = large(a->data[a1]) * b->data[b2];
//         z->data[0] = LO(reuse);
//         z->data[1] = HI(reuse);
//         z->l = 1;
//         if (HI(reuse))
//             z->l++;
//         return z;
//     }
// }

big *mulHelp(big *a, big *b) // grade school n^2 algorithm
{
    big z;
    big *c = new big();
    RANGE(i, a->l)
    {
        mulSingle(&z, a, b->data[i], i);
        *c += z;
    }
    c->sign = a->sign ^ b->sign;
    z.~big();
    return c;
}

int compare(big *a, big *b) // averagely const time, 1 if a > b
{
    RANGEr(i, SZ - 1)
    {
        if (a->data[i] > b->data[i])
            return 1;
        if (a->data[i] < b->data[i])
            return -1;
    }
    return 0; // both are equal
}

inline big *signs(big *a, big *b)
{
    big *c;
    int s = compare(a, b);
    if (s == 0)
        return new big();
    if (s == -1)
    {
        c = subHelp(b, a);
        c->sign = 1;
    }
    if (s == 1)
        c = subHelp(a, b);
    c->sign ^= a->sign;
    return c;
}

big *arithmetic(big *a, big *b, char i) // add if i == 1 , sub if i == -1
{
    big *c;
    if (i == 1)
    {
        if (a->sign != b->sign)
            return signs(a, b);
        c = addHelp(a, b);
        c->sign = a->sign;
        return c;
    }
    if (i == -1)
    {
        if (a->sign != b->sign)
        {
            c = addHelp(a, b);
            c->sign = a->sign;
            return c;
        }
        return signs(a, b);
    }
}

// operator oveloading

inline big &operator+(big &a, big &b) { return *arithmetic(&a, &b, 1); }

inline big &operator-(big &a, big &b) { return *arithmetic(&a, &b, -1); }

inline big &operator*(big &a, big &b)
{
    if (a.l > b.l)
        return *mulHelp(&a, &b);
    return *mulHelp(&b, &a);
}

inline bool operator==(big &a, big &b)
{
    if (a.sign == b.sign)
        return compare(&a, &b) == 0;
    return 0;
}

inline bool operator>(big &a, big &b)
{
    if (a.sign == b.sign)
    {
        if (a.sign > 0)
            return compare(&a, &b) == -1;
        return compare(&a, &b) == 1;
    }
    if (a.sign > b.sign)
        return 0;
    return 1;
}

inline bool operator<(big &a, big &b)
{
    if (a.sign == b.sign)
    {
        if (a.sign > 0)
            return compare(&a, &b) == 1;
        return compare(&a, &b) == -1;
    }
    if (a.sign > b.sign)
        return 1;
    return 0;
}

int main()
{
    big a("-777777777777777744444444444444781718771787444");
    big b("0");
    big c = a * b;
    //c += 100;
    string s = toDecimal(c.data, c.l, c.sign, POW);
    cout << s;
}
