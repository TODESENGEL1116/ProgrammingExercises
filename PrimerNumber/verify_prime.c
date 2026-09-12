#include <stdio.h>
#include <gmp.h>

// 使用 GMP 内置的 Miller-Rabin 素性测试
// 返回 2 = 确定是素数, 1 = 极大概率是素数, 0 = 合数
int is_prime(mpz_t n) {
    return mpz_probab_prime_p(n, 25);  // 25轮测试，误判率 < 1/4^25
}

int main() {
    unsigned long n_values[] = {2, 10, 18, 34, 58, 94, 742, 2394, 3286};
    int num_tests = sizeof(n_values) / sizeof(n_values[0]);

    mpz_t one, two, six, result, temp;
    mpz_init_set_ui(one, 1);
    mpz_init_set_ui(two, 2);
    mpz_init_set_ui(six, 6);
    mpz_init(result);
    mpz_init(temp);

    printf("%-6s | %-10s | %s\n", "n", "位数", "素性");
    printf("------------------------------------------\n");

    for (int i = 0; i < num_tests; i++) {
        unsigned long n = n_values[i];

        // result = 1^n + 6^n + 2^n
        mpz_set(result, one);                        // 1^n = 1

        mpz_pow_ui(temp, six, n);                    // 6^n
        mpz_add(result, result, temp);               // + 6^n

        mpz_pow_ui(temp, two, n);                    // 2^n
        mpz_add(result, result, temp);               // + 2^n

        int prime = is_prime(result);

        // 获取结果的十进制位数
        size_t bits = mpz_sizeinbase(result, 10);

        printf("%-6lu | %-10zu | ", n, bits);
        if (prime == 2)
            printf("✓ 确定是素数\n");
        else if (prime == 1)
            printf("✓ 极大概率是素数 (Miller-Rabin 通过)\n");
        else
            printf("✗ 不是素数\n");
    }

    mpz_clear(one);
    mpz_clear(two);
    mpz_clear(six);
    mpz_clear(result);
    mpz_clear(temp);

    return 0;
}