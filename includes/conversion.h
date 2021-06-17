#ifndef CONVERT_H
#define CONVERT_H

#include <string>
#include <bits/stdc++.h>
#include <math.h>
typedef uint32_t small;
using namespace std;

small Odd(string &s);
uint8_t odds(char c);
string divByTwo(string &s);
int convert(string &num, small *data, int pow);
inline void greaterThan5(unsigned char *data, int N);
inline void lefts(unsigned char *data, int N, unsigned char re);
unsigned char leftShift(small *data, int l, int POW);
string toDecimal(small *data, int l, bool sign, int POW);

#endif