#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint64_t u64;
typedef unsigned __int128 u128;

/* 快速幂取模: (base^exp) % mod */
u64 mod_pow(u64 base, u64 exp, u64 mod) {
    u64 result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (u128)result * base % mod;
        exp >>= 1;
        base = (u128)base * base % mod;
    }
    return result;
}

/* Miller-Rabin 素性测试（确定性版本，对 n < 2^64 精确） */
int is_prime(u64 n) {
    if (n < 2) return 0;
    if (n == 2 || n == 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;

    /* 将 n-1 分解为 2^r * d */
    u64 d = n - 1;
    int r = 0;
    while (d % 2 == 0) {
        d /= 2;
        r++;
    }

    /* 这组基底可对 n < 2^64 给出确定性结果 */
    static const u64 bases[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };

    for (int i = 0; i < 12; i++) {
        u64 a = bases[i];
        if (n <= a) break;

        u64 x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;

        int composite = 1;
        for (int j = 0; j < r - 1; j++) {
            x = (u128)x * x % n;
            if (x == n - 1) {
                composite = 0;
                break;
            }
        }
        if (composite)
            return 0;
    }
    return 1;
}

int main(void) {
    const char num[] = "1276812967623946997";
    int len = strlen(num);
    int all_prime = 1;

    printf("验证左截断素数: %s\n\n", num);

    for (int i = 0; i < len; i++) {
        /* 从第 i 位开始解析为 uint64_t */
        u64 n = 0;
        for (int j = i; j < len; j++) {
            n = n * 10 + (num[j] - '0');
        }

        int prime = is_prime(n);
        printf("%*s%-19s -> %s\n",
               i, "", num + i, prime ? "PRIME ✓" : "NOT PRIME ✗");

        if (!prime) all_prime = 0;
    }

    printf("\n结论: 所有截断数%s素数。\n", all_prime ? "都是" : "不全是");
    return 0;
}