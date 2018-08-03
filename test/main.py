#!/usr/bin/env python
import sys
import tests


def main():
    test = tests.TestMemoryLeak(None)

    if test.run():
        exit(0)
    else:
        exit(1)

if __name__ == '__main__':
    main()