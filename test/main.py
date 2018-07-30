#!/usr/bin/env python
import sys
import streams
from instance import Instance
import heapq


def test(stream, instances):

    for i in range(100):
        element = stream.next_element()
        for instance in instances:
            instance.process_element(str(element))

    k = 20

    for instance in instances:
        print(instance.k_top_query(k))
        print(instance.get_stats())

    input()
    top_k_elements = [(element, stream.elements[element]/stream.N) for element in heapq.nlargest(k, stream.elements, key=stream.elements.get)]

    print(top_k_elements)


def main():
    exec_path = sys.argv[1]
    stream_name = sys.argv[2]

    if stream_name == "zipf":
        stream = streams.Zipf(1.5)
    else:
        exit(1)

    instances = [
        Instance(exec_path, "-a lottery_sampling -m 10 -seed 1 -aging"),
        Instance(exec_path, "-a space_saving -m 10")
    ]

    test(stream, instances)


if __name__ == "__main__":
    main()