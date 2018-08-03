import sys
import os
import random
import subprocess
from abc import abstractmethod

import streams
from instance import Instance


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


class Test:

    def __init__(self, configuration='release'):
        self.exec_path = self.build_executable(configuration)


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


class TestMemoryLeak(Test):

    def __init__(self, params):
        super().__init__('debug')


    def run(self):

        for i in range(3):
            seed = self.generate_seed()

            stream = streams.Zipf(1.05, seed)

            m = 10**(i+2)

            instances = [
                Instance(self.exec_path, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile='memory_leak'),
                Instance(self.exec_path, '-a space_saving -m ' + str(m), profile='memory_leak')
            ]

            for j in range(100000):
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
                    eprint('Memory leak:', memory_leak, 'bytes')
                    eprint(instance.command)
                    eprint('Using seed:', seed)
                    return False

        return True
