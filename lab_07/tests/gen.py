import random as rnd
import string
import sys

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} count")
        exit(1)

    cnt = int(sys.argv[1])
    for _ in range(cnt):
        length = rnd.randint(1, 10)
        s = ""
        for _ in range(length):
            s += rnd.choice(string.ascii_lowercase)
        print(s)

if __name__ == "__main__":
    main()
