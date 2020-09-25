#include <bits/stdc++.h>

#define MAX_N 290000
#define NUM_BITS 20

using namespace std;

long decbin_dp[MAX_N][NUM_BITS];
long total[MAX_N];
vector<long> pows;
int sz = 0;

// Complete the decibinaryNumbers function below.
long decibinaryNumbers(long x) {
    if (x <= 0) {
        return -1;
    } else if (x == 1) {
        return 0;
    } else if (x == 2) {
        return 1;
    }
    long max_num = 1L << 54;
    if (pows.empty()) {
        pows.push_back(1);
        for (int i = 1; i < 19; i++) {
            pows.push_back(pows.back() * 10);
        }
    }
    if (sz <= 0) {
        for (int j = 0; j < NUM_BITS; j++) {
            decbin_dp[sz][j] = 1;
        }
        total[sz] = decbin_dp[sz][NUM_BITS - 1];
        ++sz;
    }
    if (sz <= 1) {
        for (int j = 1; j < NUM_BITS; j++) {
            decbin_dp[sz][j] = 1;
        }
        total[sz] = total[sz - 1] + decbin_dp[sz][NUM_BITS - 1];
        ++sz;
    }
    for (int i = 2; i < max_num; i++) {
        if (total[sz - 1] >= x) {
            break;
        }
        if (sz <= i) {
            for (int j = 1; j < NUM_BITS; j++) {
                long m = 1L << (j - 1);
                if (m <= i) {
                    for (int k = 1; k < 10; k++) {
                        if (m * k <= i) {
                            decbin_dp[i][j] += decbin_dp[i - m * k][j - 1];
                        }
                    }
                }
                decbin_dp[i][j] += decbin_dp[i][j - 1];
            }
            total[sz] = total[sz - 1] + decbin_dp[sz][NUM_BITS - 1];
            ++sz;
        }
    }
    int i = std::lower_bound(total, total + sz, x) - total;
    long result = 0;
    x -= total[i - 1];
    while (i > 0) {
        int j = 0;
        while (j < NUM_BITS && decbin_dp[i][j] < x) {
            ++j;
        }
        if (j > 0) {
            x -= decbin_dp[i][j - 1];
        }
        long m = 1L << (j - 1);
        int k = 1;
        while (k < 10 && m * k <= i && decbin_dp[i - m * k][j - 1] < x) {
            x -= decbin_dp[i - m * k][j - 1];
            ++k;
        }
        result += k * pows[j - 1];
        i -= m * k;
    }
    return result;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        long x;
        cin >> x;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        long result = decibinaryNumbers(x);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}
