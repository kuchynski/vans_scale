//---------------------------------------------------------------------------
#include "SAxel.h"
//---------------------------------------------------------------------------
SAxel::SAxel(double o, const unsigned &d, const SElementFourth<int> &c, double &s,
             const SElementFourth<int> &p, const SElementFourth<int> &t, const SElementFourth<int> &h):
  distance(d), coordinate(c), speed(s), progib(p), tare(t), progib_simple(h), om(o)
{}
//---------------------------------------------------------------------------
SAxel::SAxel(const SAxel &a)
{
    *this = a;
}
//---------------------------------------------------------------------------
SAxel& SAxel::operator=(const SAxel& a)
{
    distance = a.distance;
    coordinate = a.coordinate;
    speed = a.speed;
    progib = a.progib;
    tare = a.tare;
    progib_simple = a.progib_simple;
    om = a.om;
    return *this;
}
//---------------------------------------------------------------------------
