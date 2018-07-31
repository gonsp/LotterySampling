#!/usr/bin/env python
import sys
import streams
from instance import Instance
import heapq
import random


def test(stream, instances):

    for i in range(100000):
        element = stream.next_element()
        for instance in instances:
            instance.process_element(str(element))

    k = 10

    for instance in instances:
        print(instance.k_top_query(k))
        print(instance.get_stats())

    top_k_elements = [(element, stream.elements[element]/stream.N) for element in heapq.nlargest(k, stream.elements, key=stream.elements.get)]
    print(top_k_elements)

    print("n =", stream.n)
    input()

def main():
    exec_path = sys.argv[1]
    stream_name = sys.argv[2]

    # seed = random.randrange(10000000)
    seed = 5629943
    print("Seed was:", seed)
    if stream_name == "zipf":
        stream = streams.Zipf(2, seed)
    else:
        exit(1)

    m = 10
    instances = [
        Instance(exec_path, '-a lottery_sampling -m ' + str(m) + ' -seed 1 -aging'),
        Instance(exec_path, '-a space_saving -m ' + str(m))
    ]

    test(stream, instances)


if __name__ == "__main__":
    main()