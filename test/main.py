#!/usr/bin/env python
import sys


def main():

    mod = __import__('tests', fromlist=[sys.argv[1]])
    test = getattr(mod, sys.argv[1])(None)

    if test.run():
        exit(0)
    else:
        exit(1)

if __name__ == '__main__':
    main()