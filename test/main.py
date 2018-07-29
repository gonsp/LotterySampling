#!/usr/bin/env python
import sys
from instance import Instance


def main():
    exec_path = sys.argv[1]
    stream_name = sys.argv[2]

    instances = [
        Instance(exec_path, "-a lottery_sampling -m 10 -seed 1 -aging"),
        Instance(exec_path, "-a space_saving -m 10")
    ]
    print(instances)


if __name__ == "__main__":
    main()