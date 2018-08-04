import sys
import os
import random
import subprocess
import argparse
from abc import abstractmethod

import streams
from instance import Instance


class Test:

    def __init__(self, configuration='release', args=[]):
        self.exec_path = self.build_executable(configuration)
        arg_parser = argparse.ArgumentParser()
        for name, required in args:
            arg_parser.add_argument('-' + name, required=required)
        self.params = arg_parser.parse_args()


    @abstractmethod
    def run(self):
        pass


    def build_executable(self, configuration='release'):
        print('BUILDING EXECUTABLE FOR TEST')
        directory = '../' + 'cmake-build-' + configuration
        if not os.path.exists(directory):
            os.makedirs(directory)
        command = 'cd ' + directory + ' && cmake -DCMAKE_BUILD_TYPE=' + configuration + ' .. && make'
        process = subprocess.Popen(command, shell=True)
        process.communicate()
        if process.returncode != 0:
            exit(1)

        return directory + '/heavy_hitters'


    def generate_seed(self):
        return random.randrange(10000000)


    def error(self, *args, seed=None, command=None):
        print("ERROR", file=sys.stderr)
        print(*args, file=sys.stderr)
        if command is not None:
            print(command, file=sys.stderr)
        if seed is not None:
            print('Using seed:', seed, file=sys.stderr)
        exit(1)


class TestMemoryLeak(Test):

    def __init__(self):
        super().__init__('debug', [('seed', False)])


    def run(self):

        if self.params.seed is not None:
            seed = int(self.params.seed)
        else:
            seed = self.generate_seed()

        stream = streams.Zipf(1.05, seed)

        m = 10000

        instances = [
            Instance(self.exec_path, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile='memory_leak'),
            Instance(self.exec_path, '-a space_saving -m ' + str(m), profile='memory_leak')
        ]

        for i in range(100000):
            element = stream.next_element()
            for instance in instances:
                instance.process_element(str(element))

        for instance in instances:
            instance.k_top_query(m/2)
            instance.frequent_query(0.05)

            instance.finish()
            stats = instance.get_stats()
            memory_leak = stats['memory_leak_profiler']
            if memory_leak > 0:
                self.error('Memory leak:', memory_leak, 'bytes', seed=seed, command=instance.command)


class TestMemoryUsage(Test):

    def __init__(self):
        args = [('m', True)]
        super().__init__('debug', args)


    def run(self):
        pass


class TestMemoryUsageEvolution(Test):

    def __init__(self):
        args = []
        super().__init__('debug', args)


    def run(self):
        pass