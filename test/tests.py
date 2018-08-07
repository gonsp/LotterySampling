import math
import sys
import os
import random
import subprocess
import argparse
from abc import abstractmethod, ABCMeta
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
        configuration = 'release' if profile is None else 'debug'
        return [
            Instance('../bin/optimization-0-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile=profile),
            Instance('../bin/optimization-0-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging -multilevel', profile=profile),
            Instance('../bin/optimization-0-' + configuration, '-a space_saving -m ' + str(m), profile=profile),
            Instance('../bin/optimization-1-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile=profile),
            Instance('../bin/optimization-1-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging -multilevel', profile=profile),
            Instance('../bin/optimization-1-' + configuration, '-a space_saving -m ' + str(m), profile=profile),
            Instance('../bin/optimization-2-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile=profile),
            Instance('../bin/optimization-2-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging -multilevel', profile=profile),
            Instance('../bin/optimization-2-' + configuration, '-a space_saving -m ' + str(m), profile=profile),
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

        instances = self.create_instances(m, seed, 'memory_leak')

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


class TestAsymptotic(Test):
    # To test asymptotic behaviour of memory or exec time with respect to the intial sample size

    def __init__(self, testing='memory', profiler=False, ylabel=''):
        args = [
            ('initial_m', True),
            ('N', True),
            ('iterations', False),
            ('stream', True),
            ('seed', False)
        ]
        self.testing = testing
        self.ylabel = ylabel
        if profiler:
            if testing == 'memory':
                self.profile = 'memory_usage'
                self.stats_name = 'memory_usage_peak_profiler'
            elif testing == 'time':
                self.profile = 'exec_time'
                self.stats_name = 'process_element_cost_profiler'
        else:
            self.profile = None
            if testing == 'memory':
                self.stats_name = 'memory_usage'
            else:
                self.stats_name = 'process_element_time'
        super().__init__(configuration='debug' if profiler else 'release', args=args)


    def run(self):

        N = int(self.params.N)

        seed = self.generate_seed()

        if self.params.iterations is None:
            self.params.iterations = 10
        else:
            self.params.iterations = int(self.params.iterations)

        m_history = []
        value_history = []
        sample_size_history = []
        n_history = []

        for iteration in range(1, self.params.iterations + 1):
            print('Iteration:', iteration, '/', self.params.iterations)

            m = iteration * int(self.params.initial_m)

            if self.params.stream == 'zipf':
                stream = streams.Zipf(1.1, seed)
            elif self.params.stream == 'uniform':
                # stream = streams.Uniform(int(N/2), seed, False)   # To test the "insert_element" operation (with a few updates)
                # stream = streams.Uniform(int(1.1*m), seed, False) # To test the "update_element" operation (with a few inserts)
                stream = streams.Uniform(2*m, seed, False)          # In expectation there will be N/2 inserts and N/2 updates.

            instances = self.create_instances(m, seed, self.profile)

            for i in range(N):
                element = stream.next_element()
                for instance in instances:
                    instance.process_element(str(element))

            results = []
            sizes = []
            for instance in instances:
                instance.k_top_query(m)
                instance.frequent_query(0.05)

                instance.finish()
                stats = instance.get_stats()
                value = stats[self.stats_name]
                if self.testing == 'time':
                    value = value / N
                results.append(value)
                sizes.append(stats['sample_size'])

            m_history.append(m)
            value_history.append(results)
            sample_size_history.append(sizes)
            n_history.append(stream.n)

        m_history = np.array(m_history)
        value_history = np.array(value_history)
        sample_size_history = np.array(sample_size_history)
        n_history = np.array(n_history)

        print('Showing results')
        _, axes = plt.subplots()
        axes_right = axes.twinx()

        for i, instance in enumerate(instances):
            axes.plot(m_history, value_history[:, i], '-o', label=self.testing + ' ' + instance.name)
            axes_right.plot(m_history, sample_size_history[:, i], '--x', label='Sample size')
        axes_right.plot(m_history, m_history * np.log(n_history/m_history), 'm:.', label='m * ln(n/m)')

        axes.legend(loc='upper left')
        axes_right.legend(loc='upper right')
        axes.set_xlabel('m (Parameter for number of sampled elements)')
        axes.set_ylabel(self.ylabel)
        axes_right.set_ylabel('Sample size')

        self.full_screen_plot()
        plt.show()


class TestAsymptoticMemoryProfiler(TestAsymptotic):

    def __init__(self):
        super().__init__(testing='memory', profiler=True, ylabel='Max used memory (bytes)')


class TestAsymptoticMemory(TestAsymptotic):

    def __init__(self):
        super().__init__(testing='memory', profiler=False, ylabel='Used memory (bytes)')


class TestAsymptoticTimeProfiler(TestAsymptotic):

    def __init__(self):
        super().__init__(testing='time', profiler=True, ylabel='Cost (Valgrind output)')


class TestAsymptoticTime(TestAsymptotic):

    def __init__(self):
        super().__init__(testing='time', profiler=False, ylabel='Mean exec time for element in the stream (microseconds)')


class TestMemoryUsageEvolution(Test):
    # Test to see the evolution of memory usage through the entire stream

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

        iterations = []
        memory = []
        sample_size = []
        n = []

        for i in range(int(self.params.N)):
            element = stream.next_element()

            for instance in instances:
                instance.process_element(str(element))

            if i % int(self.params.sample_period) == 0:
                iterations.append(i)
                n.append(stream.n)
                memory_usage = []
                size = []
                for instance in instances:
                    stats = instance.get_stats()
                    memory_usage.append(stats['memory_usage'])
                    size.append(stats['sample_size'])
                memory.append(memory_usage)
                sample_size.append(size)

        iterations = np.array(iterations)
        memory = np.array(memory)
        sample_size = np.array(sample_size)
        n = np.array(n)

        _, axes = plt.subplots()
        axes_right = axes.twinx()

        for i, instance in enumerate(instances):
            axes.plot(iterations, memory[:, i], '-', label=instance.name)
            axes_right.plot(iterations, sample_size[:, i], 'y--', label='Sample size')
        axes_right.plot(iterations, m * np.log(n/m), 'm:.', label='m * ln(n/m)')

        axes.legend(loc='upper left')
        axes_right.legend(loc='upper right')
        axes.set_xlabel('Number of elements read from the stream')
        axes.set_ylabel('Used memory (bytes)')
        axes_right.set_ylabel('Number of elements')
        self.full_screen_plot()
        plt.show()

