"""transposition_tableのhash_array_用乱数生成."""
import itertools
import random


def main():
    """メイン関数."""
    for r in itertools.islice(iterrandom(), 2 * 8 * 256):
        print(f'{r:#018x},')


def iterrandom():
    """64bits整数列."""
    while True:
        yield random.randint(0, 0xffffffffffffffff)


if __name__ == '__main__':
    main()
