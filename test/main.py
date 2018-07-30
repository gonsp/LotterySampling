#!/usr/bin/env python
import sys
import streams
from instance import Instance


def test(stream, instances):

    for i in range(100):
        element = stream.next_element()
        print(element)
        for instance in instances:
            instance.process_element(str(element))

    for instance in instances:
        print(instance)
        print(instance.k_top_query(10))
        print(instance.get_stats())


def main():
    exec_path = sys.argv[1]
    stream_name = sys.argv[2]

    if stream_name == "zipf":
        stream = streams.Zipf(2)
    else:
        exit(1)

    instances = [
        Instance(exec_path, "-a lottery_sampling -m 10 -seed 1 -aging"),
        Instance(exec_path, "-a space_saving -m 10")
    ]

    test(stream, instances)


if __name__ == "__main__":
    main()