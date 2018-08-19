#!/usr/bin/env python
import sys


def main():

    test_name = sys.argv[1]
    test_name = 'Test' + test_name

    mod = __import__('tests', fromlist=[test_name])
    test = getattr(mod, test_name)()

    test.run()
    exit(0)

if __name__ == '__main__':
    main()