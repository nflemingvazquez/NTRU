# include <complex>
# include "Poly.h"
#include <math.h>
# define PI 3.14159265358979323846
complex<double> * polyToFFTArray(Poly a, size_t * arrLengthPtr);
complex<double> * recursiveFFT(complex<double> * arr, size_t arrLength);

/* Implementation of polynomial multiplication using FFT,
based on Chapter 30 of Introduction to Algorithms */

complex<double> * polyToFFTArray(Poly a, size_t * arrLengthPtr)
{
	
}

complex<double> * recursiveFFT(const complex<double> * arr, size_t arrLength)
{
	size_t n = arrLength;
	if (n == 1) { // shorten this code!
		complex<double> * y = new complex<double>[1];
		complex<double> a(1,0);
		y[0] = a;
		return y;
	}
	complex<double> om_n(cos(2*PI/n), sin(2 * PI / n));
	complex<double> * a_0 = new complex<double>[n / 2];
	complex<double> * a_1 = new complex<double>[n / 2];
	for (int i = 0; i < n; ++i) {
		a_0[i] = arr[2 * n];
		a_1[i] = arr[2 * n + 1];
	}
	complex<double> * y_0 = recursiveFFT(a_0, n/2);
	complex<double> * y_1 = recursiveFFT(a_1, n/2);
	complex<double> * y = new complex<double>[n];
	complex<double>om = 1;
	for (int j = 0; j < n / 2; ++j) {
		y[j] = y_0[j] + om * y_1[j];
		y[j + n / 2] = y_0[j] - om*y_1[j];
		om *= om_n;
	}
	delete[] a_0;
	delete[] a_1;
	delete[] y_0;
	delete[] y_1;
	return y;
}


