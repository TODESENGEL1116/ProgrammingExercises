/*
 * 验证两个无穷级数恒等式（GMP 高精度版）
 * 编译: g++ verify.cpp -lgmpxx -lgmp -o verify
 * 运行: ./verify
 */
#include <iostream>
#include <iomanip>
#include <vector>
#include <gmpxx.h>

using namespace std;

// ========== k-step Fibonacci（记忆化） ==========
class KFib {
    int k;
    vector<mpz_class> cache;
public:
    KFib(int k_) : k(k_) { cache.push_back(0); } // cache[0]=0

    mpz_class get(int n) {
        if (n <= 0) return 0;
        if (n == 1) return 1;
        while ((int)cache.size() <= n) {
            int sz = cache.size();
            mpz_class sum = 0;
            for (int j = 1; j <= k && sz - j >= 0; j++)
                sum += cache[sz - j];
            cache.push_back(sum);
        }
        return cache[n];
    }
};

// 二项式系数 C(m,j)
inline mpz_class C(unsigned long m, unsigned long j) {
    mpz_class res;
    mpz_bin_uiui(res.get_mpz_t(), m, j);
    return res;
}

// 广义调和数 H_n^(r)
mpf_class harmonic(int n, int r) {
    if (n <= 0) return mpf_class(0);
    mpf_class sum = 0;
    for (int j = 1; j <= n; j++) {
        mpf_class term = 1;
        for (int p = 0; p < r; p++) term /= mpf_class(j);
        sum += term;
    }
    return sum;
}

int main() {
    mpf_set_default_prec(1024);  // ~300位十进制精度
    mpf_class target(268);

    cout << fixed << setprecision(15);

    // ====== 公式 1 ======
    cout << "Formula 1:\n";
    cout << string(60, '-') << "\n";
    KFib F5(5);
    mpf_class total1 = 0;

    for (int n = 1; n <= 60; n++) {
        mpz_class comb2n = C(2 * n, n);
        mpf_class h = harmonic(n + 1, 2);

        mpz_class t1 = 16; t1 *= (n + 1); t1 *= (n + 1); t1 *= (n + 1); t1 *= F5.get(n + 5);
        mpz_class t2 = 4;  t2 *= (2 * n + 1); t2 *= (2 * n * n + 6 * n + 5); t2 *= F5.get(n + 6);
        mpz_class t3 = 1;  t3 *= (n + 2); t3 *= (2 * n + 1); t3 *= (2 * n + 3); t3 *= F5.get(n + 7);
        mpz_class bracket = t1 - t2 + t3;

        mpz_class denom; mpz_ui_pow_ui(denom.get_mpz_t(), 2, 3 * n - 1); denom *= (n + 1);

        mpf_class term = mpf_class(comb2n) * h * mpf_class(bracket) / mpf_class(denom);
        total1 += term;

        cout << "n=" << setw(2) << n << "  partial = " << total1 << "\n";
    }
    cout << "→ target = 268,  result = " << total1 << "\n\n";

    // ====== 公式 2 ======
    cout << "Formula 2:\n";
    cout << string(60, '-') << "\n";
    KFib F3(3);
    mpf_class total2 = 0;

    for (int n = 1; n <= 40; n++) {
        mpf_class h = harmonic(n + 1, 4);

        mpz_class t1 = 1024;
        for (int i = 0; i < 4; i++) t1 *= (n + 1);
        t1 *= C(4 * n, 2 * n);
        t1 *= F3.get(n);

        mpz_class t2 = 32;
        mpz_class p1 = 1, p2 = 1;
        for (int i = 0; i < 4; i++) { p1 *= (n + 1); p2 *= (n + 2); }
        t2 *= (p1 + p2);
        t2 *= C(4 * n + 4, 2 * n + 2);
        t2 *= F3.get(n + 1);

        mpz_class t3 = 1;
        for (int i = 0; i < 4; i++) t3 *= (n + 2);
        t3 *= C(4 * n + 8, 2 * n + 4);
        t3 *= F3.get(n + 2);

        mpz_class bracket = t1 - t2 + t3;

        mpz_class denom; mpz_ui_pow_ui(denom.get_mpz_t(), 2, 5 * n + 3);

        mpf_class term = h * mpf_class(bracket) / mpf_class(denom);
        total2 += term;

        cout << "n=" << setw(2) << n << "  partial = " << total2 << "\n";
    }
    cout << "→ target = 268,  result = " << total2 << "\n";

    return 0;
}