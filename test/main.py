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

    instances[1].process_element("1")
    print(instances[1].frequent_query(0))
    instances[1].process_element("2")
    print(instances[1].frequent_query(0))
    print(instances[1].k_top_query(1))

    print(type(instances[1].get_stats()))

if __name__ == "__main__":
    main()