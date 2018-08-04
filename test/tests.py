import math
import sys
import os
import random
import subprocess
import argparse
from abc import abstractmethod
import matplotlib.pyplot as plt
import numpy as np

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


    def create_instances(self, m, seed, profile=None):
        return [
            Instance(self.exec_path, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile=profile),
            Instance(self.exec_path, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging -multilevel', profile=profile),
            Instance(self.exec_path, '-a space_saving -m ' + str(m), profile=profile)
        ]


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
        if self.params.seed is not None:
            return int(self.params.seed)
        else:
            return random.randrange(10000000)


    def error(self, *args, seed=None, command=None):
        print("ERROR", file=sys.stderr)
        print(*args, file=sys.stderr)
        if command is not None:
            print(command, file=sys.stderr)
        if seed is not None:
            print('Using seed:', seed, file=sys.stderr)
        exit(1)


    def full_screen_plot(self):
        if plt.get_backend() == 'TkAgg':
            mng = plt.get_current_fig_manager()
            mng.resize(*mng.window.maxsize())


class TestMemoryLeak(Test):

    def __init__(self):
        super().__init__('debug', [('seed', False)])


    def run(self):

        seed = self.generate_seed()

        stream = streams.Zipf(1.05, seed)

        m = 10000

        instances = create_instances(m, seed, 'memory_leak')

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
        args = [
            ('m', True),
            ('stream', True),
            ('N', True),
            ('seed', False)
        ]
        super().__init__('debug', args)
        self.print_results = True


    def run(self):

        seed = self.generate_seed()

        if self.params.stream == 'zipf':
            stream = streams.Zipf(1.1, seed)

        m = int(self.params.m)

        self.instances = self.create_instances(m, seed, 'memory_usage')

        for i in range(int(self.params.N)):
            element = stream.next_element()
            for instance in self.instances:
                instance.process_element(str(element))

        results = []
        sizes = []
        for instance in self.instances:
            instance.k_top_query(m/2)
            instance.frequent_query(0.05)

            instance.finish()
            stats = instance.get_stats()
            memory_peak = stats['memory_usage_peak_profiler']
            if self.print_results:
                print('Memory peak:', memory_peak, instance.command)
            results.append(memory_peak)
            sizes.append(stats['sample_size'])

        return results, sizes, stream.n


class TestMemoryUsageAsymptotic(TestMemoryUsage):

    def __init__(self):
        super().__init__()
        self.print_results = False
        self.params.seed = self.generate_seed()


    def run(self):
        x = []
        y = []
        z = []
        expected_size_lottery_multilevel = []

        initial_m = self.params.m
        for i in range(0, 10):
            m = i * initial_m
            x.append(m)
            self.params.m = m
            results, sizes, n = super().run()
            y.append(results)
            z.append(sizes)
            expected_size_lottery_multilevel.append(m * math.log(n/m))

        print("Showing results")

        x = np.array(x)  # m
        y = np.array(y)  # memory
        z = np.array(z)  # sample size

        _, axes = plt.subplots()
        axes_right = axes.twinx()

        for i, instance in enumerate(self.instances):
            axes.plot(x, y[:, i], '-o', label='Memory ' + instance.name)
            axes_right.plot(x, z[:, i], '--x', label='Sample size ' + instance.name)
        axes_right.plot(x, expected_size_lottery_multilevel, 'm--x', label='Expected sample size lottery_sampling -multilevel')

        axes.legend(loc='upper left')
        axes_right.legend(loc='upper right')
        axes.set_xlabel('m (Initial number of sampled elements)')
        axes.set_ylabel('Max used memory (bytes)')
        axes_right.set_ylabel('Sample size')

        self.full_screen_plot()
        plt.show()


class TestMemoryUsageEvolution(Test):

    def __init__(self):
        args = [
            ('m', True),
            ('stream', True),
            ('N', True),
            ('sample_period', True),
            ('seed', False)
        ]
        super().__init__('release', args)


    def run(self):

        seed = self.generate_seed()

        m = int(self.params.m)

        stream = streams.Zipf(1.1, seed)

        instances = self.create_instances(m, seed)

        x = []
        y = []
        z = []
        for i in range(int(self.params.N)):
            element = stream.next_element()

            for instance in instances:
                instance.process_element(str(element))

            if i % int(self.params.sample_period) == 0:
                x.append(i)
                z.append(stream.n)
                memory_usage = []
                for instance in instances:
                    memory_usage.append(instance.get_stats()['memory_usage'])
                y.append(memory_usage)

        _, axes = plt.subplots()
        axes_right = axes.twinx()

        y = np.array(y)
        z = np.array(z)
        for i, instance in enumerate(instances):
            axes.plot(x, y[:, i], '-', label=instance.name)
        # axes_right.plot(x, z, 'y--', label='n (Cardinality)')
        axes_right.plot(x, m * np.log(z/m), 'm--', label='m * ln(n/m)')

        axes.legend(loc='upper left')
        axes_right.legend(loc='upper right')
        axes.set_xlabel('Number of elements read from the stream')
        axes.set_ylabel('Used memory (bytes)')
        axes_right.set_ylabel('Number of elements')
        self.full_screen_plot()
        plt.show()