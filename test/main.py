#!/usr/bin/env python
import sys
import streams
from instance import Instance
import heapq


def test(stream, instances):

    for i in range(1000):
        element = stream.next_element()
        for instance in instances:
            instance.process_element(str(element))

    k = 10

    for instance in instances:
        # print(instance.k_top_query(k))
        stats = instance.get_stats()
        print(stats)
        instance.finish()
        # instance.print_state()

    top_k_elements = [(element, stream.elements[element]/stream.N) for element in heapq.nlargest(k, stream.elements, key=stream.elements.get)]
    print(top_k_elements)

    # print('n =', stream.n)
    input()


def main():
    exec_path = sys.argv[1]
    stream_name = sys.argv[2]

    # seed = random.randrange(10000000)
    seed = 100000;
    print('Using seed:', seed)
    if stream_name == "zipf":
        stream = streams.Zipf(1.05, seed)
    else:
        exit(1)

    m = 100
    profiling = False

    instances = [
        Instance(exec_path, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile=profiling),
        Instance(exec_path, '-a space_saving -m ' + str(m), profile=profiling)
    ]

    for i, instance in enumerate(instances):
        print('Instance', i, 'pid:', instance.pid)

    print('Starting test')
    test(stream, instances)


if __name__ == '__main__':
    main()