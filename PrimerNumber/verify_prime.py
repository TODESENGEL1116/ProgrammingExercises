def is_prime(n, k=25):
    """Miller-Rabin 素性测试"""
    if n < 2: return False
    if n in (2, 3): return True
    if n % 2 == 0: return False
    r, s = 0, n - 1
    while s % 2 == 0:
        r += 1
        s //= 2
    import random
    for _ in range(k):
        a = random.randrange(2, n - 1)
        x = pow(a, s, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

for n in [2, 10, 18, 34, 58, 94, 742, 2394, 3286]:
    val = 1**n + 6**n + 2**n
    result = "Prime" if is_prime(val) else "Not Prime"
    print(f"n={n:<4} | 位数={len(str(val)):<4} | {result}")