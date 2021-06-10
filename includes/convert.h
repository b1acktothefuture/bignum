#pragma once

#include <string>
#include <bits/stdc++.h>
#include <math.h>
typedef uint32_t small;
using namespace std;

uint8_t odds(char c);

string divByTwo(string &s);

uint32_t Odd(string &s);

int convert(string &num, uint32_t *data, int pow);

uint32_t Odd(string &s)
{
    if ((s[s.size() - 1] - '0') % 2 == 1)
        return 1;
    return 0;
}

uint8_t odds(char c)
{
    if ((c - '0') % 2 == 1)
        return 1;
    return 0;
}

string divByTwo(string &s)
{
    string ret = "";
    int add = 0;
    int dgt;
    char c;
    for (int i = 0; i < s.size(); i++)
    {
        dgt = (s[i] - '0') / 2 + add;
        c = dgt + 48;
        ret = ret + c;
        add = odds(s[i]) * 5;
    }
    if (ret != "0" && ret[0] == '0')
    {
        ret = ret.substr(1, ret.size() - 1);
    }
    return ret;
}

int convert(string &num, uint32_t *data, int pow)
{
    if (num == "0")
        return 1;
    uint32_t ret = 0;
    char count = 0;
    int i = 0;
    while (num != "0")
    {
        ret = ret | Odd(num) << count;
        count++;
        num = divByTwo(num); //optimize to reduce new string creation
        if (count == pow)
        {
            count = 0;
            data[i] = ret;
            ret = 0;
            i++;
        }
    }
    if (ret != 0)
    {
        data[i] = ret;
        i++;
    }
    return i;
}

///////////////////////////////////////////////////

inline void greaterThan5(unsigned char *data, int N)
{
    for (int i = 0; i < N; i++)
        if (data[i] >= 5)
            data[i] += 3 % 16;
}

inline void lefts(unsigned char *data, int N, unsigned char re)
{
    uint8_t reuse = 0;
    uint8_t helper;
    for (int i = 0; i < N; i++)
    {
        helper = data[i];
        helper = helper << 1;
        data[i] = helper & (15) | reuse;
        reuse = helper >> 4;
    }
    data[0] = data[0] | re;
}

unsigned char leftShift(uint32_t *data, int l, int POW)
{
    uint64_t reuse = 0;
    uint64_t helper = 0;
    for (int i = 0; i < l; i++)
    {
        helper = data[i];
        helper = helper << 1;
        data[i] = helper & ((1U << POW) - 1U) | reuse;
        reuse = helper >> POW;
    }
    return reuse;
}

string toDecimal(uint32_t *data, int l, bool sign, int POW)
{
    int n = POW * (l);
    int N = ceil(float(n) / 3.0);
    unsigned char *BCD = new unsigned char[N];
    for (int i = 0; i < N; i++)
        BCD[i] = 0;
    for (int i = 0; i < n - 1; i++)
    {
        bool re = leftShift(data, l, POW); // this seems to be working
        lefts(BCD, N, re);                 // this is definitely working
        greaterThan5(BCD, N);              // works
    }
    bool re = leftShift(data, l, POW);
    lefts(BCD, N, re);

    string s = "";
    bool t = 1;
    for (int i = N - 1; i >= 0; i--)
    {
        if (t & BCD[i] == 0)
            continue;
        t = 0;
        s = s + char(int(BCD[i]) + 48);
    }
    if (s == "")
        return "0";
    if (sign)
        s = "-" + s;
    return s;
}