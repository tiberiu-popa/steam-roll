#include <algorithm>
#include <complex>
#include <numeric>
#include <vector>
#include <cmath>
#include <cstdio>

#define EPSILON 1e-5

using namespace std;

int skip_whitespace()
{
	int ch;
	while (true) {
		ch = getchar_unlocked();
		if (ch != ' ' && ch != '\n')
			break;
	}
	return ch;
}

template<typename T>
T read_unsigned_integer()
{
	T result = (T) 0;
	int ch = skip_whitespace();
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return result;
}

template<typename T>
T read_signed_integer()
{
	T result = (T) 0;
	bool flip = false;
	int ch = skip_whitespace();
	if (ch == '-') {
		flip = true;
		ch = skip_whitespace();
	}
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return flip ? -result : result;
}

complex<double> evalpoly(const vector< complex<double> > &poly, complex<double> x,
	size_t k, size_t step)
{
	complex<double> result;
	while (true) {
		result = result * x + poly[k];
		if (k >= step)
			k -= step;
		else
			break;
	}
	return result;
}

void recursive_dft(const vector< complex<double> > &in, vector< complex<double> > &out,
	size_t n, size_t k, size_t step, double base_angle)
{
	size_t m = n / step;
	complex<double> angle(0.0, base_angle / m);
	complex<double> base = exp(angle);
	complex<double> w(1.0, 0.0);

	if (m <= 8) {
		size_t p = k + (m - 1) * step;
		for (size_t i = 0; i < m; i++) {
			out.push_back(evalpoly(in, w, p, step));
			w *= base;
		}
		return;
	}

	vector< complex<double> > subout[2];
	recursive_dft(in, subout[0], n, k, 2 * step, base_angle);
	recursive_dft(in, subout[1], n, k + step, 2 * step, base_angle);

	out.resize(m);

	for (size_t i = 0; i < m / 2; i++) {
		out[i] = subout[0][i] + w * subout[1][i];
		out[i + m / 2] = subout[0][i] - w * subout[1][i];
		w *= base;
	}
}

void reverse_bits(const vector< complex<double> > &in, vector< complex<double> > &out)
{
	size_t n = in.size();
	size_t j = 0;

	out.resize(n);
	for (size_t i = 0; i < n; i++) {
		out[i] = in[j];
		size_t m = n / 2;
		while (m > 0 && m <= j) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
}

void iterative_dft(const vector< complex<double> > &in, vector< complex<double> > &out,
	size_t n, double base_angle)
{
	size_t step = 1;

	reverse_bits(in, out);
	while (step < n) {
		size_t dstep = step << 1;
		complex<double> angle(0.0, base_angle / dstep);
		complex<double> base = exp(angle);
		complex<double> w(1.0, 0.0);
		for (size_t k = 0; k < step; k++) {
			for (size_t i = k; i < n; i += dstep) {
				size_t j = i + step;
				complex<double> a = out[i] + w * out[j];
				complex<double> b = out[i] - w * out[j];
				out[i] = a;
				out[j] = b;
			}
			w *= base;
		}
		step = dstep;
	}
}

void fft(const vector< complex<double> > &in, vector< complex<double> > &out)
{
	size_t n = in.size();
	out.clear();

	//iterative_dft(in, out, n, 2 * M_PI);
	recursive_dft(in, out, n, 0, 1, 2 * M_PI);
}

void ifft(const vector< complex<double> > &in, vector< complex<double> > &out)
{
	size_t n = in.size();
	out.clear();

	//iterative_dft(in, out, n, -2 * M_PI);
	recursive_dft(in, out, n, 0, 1, -2 * M_PI);

	for (size_t i = 0; i < out.size(); i++)
		out[i] /= n;
}

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

long long quadratic(const vector<long long> &v)
{
	vector<long long> u;
	vector<long long> numbers;

	u.push_back(0LL);
	for (size_t i = 0; i < v.size(); i++)
		u.push_back(u.back() + v[i]);

	for (size_t i = 0; i < u.size(); i++)
		for (size_t j = i + 1; j < u.size(); j++)
			numbers.push_back(u[j] - u[i]);

	sort(numbers.begin(), numbers.end());
	trim_unique(numbers);

	return (long long) numbers.size();
}

long long logarithmic(const vector<long long> &v)
{
	vector< complex<double> > y;
	vector< complex<double> > a, b;
	size_t total_sum = (size_t) accumulate(v.begin(), v.end(), 0LL);
	y.resize(total_sum + 1);

	size_t m = 1;
	while (m <= total_sum)
		m <<= 1;
	m <<= 1;

	long long sum = 0;
	y[(size_t) sum] = 1;
	for (size_t i = 0; i < v.size(); i++) {
		sum += v[i];
		y[(size_t) sum] = complex<double>(1.0, 0.0);
	}

	y.resize(m);
	fft(y, a);

	complex<double> angle(0.0, 2 * M_PI * total_sum / m);
	complex<double> base = exp(angle);
	complex<double> w(1.0, 0.0);

	for (size_t i = 0; i < a.size(); i++) {
		a[i] = norm(a[i]) * w;
		w *= base;
	}

	ifft(a, b);

	long long result = 0;
	for (size_t i = 0; i < total_sum; i++)
		if (fabs(b[i].real()) >= EPSILON)
			++result;

	return result;
}

int solve_problem()
{
	vector<long long> v;
	int n;

	n = read_unsigned_integer<long long>();
	for (int i = 0; i < n; i++)
		v.push_back(read_unsigned_integer<long long>());

	long long sum = accumulate(v.begin(), v.end(), 0LL);

	long long result = (sum > (1LL << 21)) ? quadratic(v) : logarithmic(v);
	printf("%lld\n", result - 1);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
