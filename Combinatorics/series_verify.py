"""
验证两个无穷级数恒等式：
    268 = Σ [C(2n,n)·H_{n+1}^{(2)} / 2^{3n-1}(n+1)] × [ ... ]
    268 = Σ [H_{n+1}^{(4)} / 2^{5n+3}] × [ ... ]
"""
from decimal import Decimal, getcontext
from math import comb
from functools import lru_cache

getcontext().prec = 200  # 200位十进制精度

# ========== k-step Fibonacci ==========
@lru_cache(maxsize=None)
def F_k(n: int, k: int) -> int:
    """F_m^(k) = 0 (m≤0), F_1^(k) = 1, F_n^(k) = Σ_{j=1}^k F_{n-j}^(k)"""
    if n <= 0:
        return 0
    if n == 1:
        return 1
    return sum(F_k(n - j, k) for j in range(1, k + 1))

# ========== 广义调和数 H_n^(r) ==========
def H(n: int, r: int) -> Decimal:
    """H_n^(r) = Σ_{j=1}^n 1/j^r"""
    if n <= 0:
        return Decimal('0')
    return sum(Decimal(1) / Decimal(j) ** r for j in range(1, n + 1))

# ========== 公式 1 ==========
def formula1(N: int) -> Decimal:
    """验证第一个级数收敛到 268"""
    total = Decimal('0')
    print(f"{'n':>4} | {'a_n':>28} | {'部分和':>30}")
    print("-" * 75)
    for n in range(1, N + 1):
        # 二项式系数（精确整数）
        C = comb(2 * n, n)
        # 广义调和数
        h = H(n + 1, 2)
        # 括号内三项（全部用精确整数运算）
        t1 = 16 * (n + 1) ** 3 * F_k(n + 5, 5)
        t2 = 4 * (2 * n + 1) * (2 * n ** 2 + 6 * n + 5) * F_k(n + 6, 5)
        t3 = (n + 2) * (2 * n + 1) * (2 * n + 3) * F_k(n + 7, 5)
        bracket = t1 - t2 + t3

        # 分母
        denom = Decimal(2) ** (3 * n - 1) * Decimal(n + 1)

        # 单项 = C × h × bracket / denom
        term = Decimal(C) * h * Decimal(bracket) / denom
        total += term

        print(f"{n:>4} | {term:>28.10f} | {total:>30.15f}")
    return total

# ========== 公式 2 ==========
def formula2(N: int) -> Decimal:
    """验证第二个级数收敛到 268"""
    total = Decimal('0')
    print(f"{'n':>4} | {'b_n':>28} | {'部分和':>30}")
    print("-" * 75)
    for n in range(1, N + 1):
        h = H(n + 1, 4)

        # 括号内三项（精确整数）
        t1 = 1024 * (n + 1) ** 4 * comb(4 * n, 2 * n) * F_k(n, 3)
        t2 = 32 * ((n + 1) ** 4 + (n + 2) ** 4) * comb(4 * n + 4, 2 * n + 2) * F_k(n + 1, 3)
        t3 = (n + 2) ** 4 * comb(4 * n + 8, 2 * n + 4) * F_k(n + 2, 3)
        bracket = t1 - t2 + t3

        # 分母
        denom = Decimal(2) ** (5 * n + 3)

        term = h * Decimal(bracket) / denom
        total += term

        print(f"{n:>4} | {term:>28.10f} | {total:>30.15f}")
    return total

if __name__ == "__main__":
    print("=" * 75)
    print("Formula 1: 验证 268 = Σ ... F^(5) ...")
    print("=" * 75)
    r1 = formula1(80)
    print(f"\n→ 前80项部分和 = {r1:.20f}\n")

    print("=" * 75)
    print("Formula 2: 验证 268 = Σ ... F^(3) ...")
    print("=" * 75)
    r2 = formula2(80)
    print(f"\n→ 前80项部分和 = {r2:.20f}\n")