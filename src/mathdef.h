#ifndef mathdef_h
#define mathdef_h

#include <math.h>
#include "vector.h"

#define m_pi 3.14159
#define m_epsilon 0.000001

#define m_sqr(n) (n*n)
#define m_cube(n) (n*n*n)

#define m_abs(n) (n < 0.0 ? -n : n)
#define m_min(m, n) (m < n ? m : n)
#define m_max(m, n) (m < n ? n : m)

#define m_nan(n) (!(n > 0 || n <= 0))

#define m_clamp(n, a, b) m_min(m_max(n, a), b)

#define m_even(n) (int(n/2) == double(n)/2.0 ? true : false)
#define m_odd(n) (int(n/2) == double(n)/2.0 ? false : true)

#define degtorad(a) m_pi*a/180.0
#define radtodeg(a) 180.0*a/m_pi

#define noise_B 0x100
#define noise_BM 0xff
#define noise_N 0x1000
#define noise_NP 12   /* 2^N */
#define noise_NM 0xfff

#define s_curve(t) ( t * t * (3. - 2. * t) )
#define lerp(t, a, b) ( a + t * (b - a) )
#define setup(i,b0,b1,r0,r1)\
        t = vec[i] + noise_N;\
        b0 = ((int)t) & noise_BM;\
        b1 = (b0+1) & noise_BM;\
        r0 = t - (int)t;\
        r1 = r0 - 1.;
#define at2(rx,ry) ( rx * q[0] + ry * q[1] )
#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

void noiseinit(void);
double noise1(double);
double noise2(double *);
double noise3(double *);
void normalize3(double *);
void normalize2(double *);

double PerlinNoise1D(double,double,double,int);
double PerlinNoise2D(double,double,double,double,int);
double PerlinNoise3D(double,double,double,double,double,int);

void noise3tex(unsigned char *tex, int size);

int intraytri(vec orig, vec dir, vec vert0, vec vert1, vec vert2, double *t, double *u, double *v);

#endif
