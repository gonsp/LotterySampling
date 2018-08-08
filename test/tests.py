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
import metrics
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
    # To test asymptotic behaviour of memory, exec time or accuracy (and sample size) with respect to the initial sample size

    def __init__(self, metric_1='memory', metric_2='', profiler=False, y_label='', y_right_label=None):
        args = [
            ('initial_m', True),
            ('N', True),
            ('iterations', True),
            ('seed', False),
            ('k', False),
            ('freq', False)
        ]
        self.y_label = y_label
        self.y_right_label = y_right_label
        self.metric_1 = metric_1
        self.metric_2 = metric_2
        if profiler:
            if metric_1 == 'memory':
                self.profile = 'memory_usage'
            elif metric_1 == 'time':
                self.profile = 'exec_time'
            else:
                exit(1)
        else:
            self.profile = None

        super().__init__(configuration='debug' if profiler else 'release', args=args)


    def run(self):

        N = int(self.params.N)

        seed = self.generate_seed()

        if self.params.iterations is None:
            self.params.iterations = 10
        else:
            self.params.iterations = int(self.params.iterations)

        m_history = []
        metric_1_history = []
        metric_2_history = []
        sample_size_history = []
        n_history = []

        for iteration in range(1, self.params.iterations + 1):
            print('Iteration:', iteration, '/', self.params.iterations)

            m = iteration * int(self.params.initial_m)

            stream = self.get_stream(iteration, m, N)

            instances = self.create_instances(m, seed, self.profile)

            for i in range(N):
                element = stream.next_element()
                for instance in instances:
                    instance.process_element(str(element))

            results_metric_1 = []
            results_metric_2 = []
            sizes = []
            for instance in instances:
                if self.profile is not None:
                    instance.finish()
                metric_1 = self.get_metric_1(instance, stream)
                metric_2 = self.get_metric_2(instance, stream)
                instance.finish()
                results_metric_1.append(metric_1)
                results_metric_2.append(metric_2)
                sizes.append(instance.get_stats()['sample_size'])

            m_history.append(m)
            metric_1_history.append(results_metric_1)
            metric_2_history.append(results_metric_2)
            sample_size_history.append(sizes)
            n_history.append(stream.n)

        m_history = np.array(m_history)
        metric_1_history = np.array(metric_1_history)
        metric_2_history = np.array(metric_2_history)
        sample_size_history = np.array(sample_size_history)
        n_history = np.array(n_history)

        print('Showing results')
        _, axes = plt.subplots()
        axes_right = axes.twinx()

        for i, instance in enumerate(instances):
            axes.plot(m_history, metric_1_history[:, i], '-*', label=self.metric_1 + ' ' + instance.name)
            if self.y_right_label is None:
                axes_right.plot(m_history, sample_size_history[:, i], '--', label='Sample size')

        if self.y_right_label is None:
            axes_right.plot(m_history, m_history * np.log(n_history/m_history), 'm:', label='m * ln(n/m)')

        if all([value is not None for value in metric_2_history.flatten()]):
            axes.set_prop_cycle(None)
            for i, instance in enumerate(instances):
                metric_2_axes = axes if self.y_right_label is None or self.y_right_label == '' else axes_right
                metric_2_axes.plot(m_history, metric_2_history[:, i], '-.x', label=self.metric_2 + ' ' + instance.name)

        axes.legend(loc='upper left')
        axes_right.legend(loc='upper right')
        axes.set_xlabel('m (Parameter for number of sampled elements)')
        axes.set_ylabel(self.y_label)
        axes_right.set_ylabel('Sample size' if self.y_right_label is None else self.y_right_label)

        self.full_screen_plot()
        plt.show()

    @abstractmethod
    def get_stream(self, iteration, m, N):
        pass


    @abstractmethod
    def get_metric_1(self, instance, stream):
        pass


    def get_metric_2(self, instance, stream):
        return None


class TestAsymptoticMemoryProfiler(TestAsymptotic):
    # Tests the total memory allocated ONLY by the algorithm. It gets very accurate results, although maybe the
    # other version suffices to get good enough results.

    def __init__(self):
        super().__init__(metric_1='memory', profiler=True, y_label='Max used memory (bytes)')


    def get_stream(self, iteration, m, N):
        return streams.Zipf(1.1, self.generate_seed(), save=False)


    def get_metric_1(self, instance, stream):
        return instance.get_stats()['memory_usage_peak_profiler']


class TestAsymptoticMemory(TestAsymptotic):
    # Faster than the profiler version but the memory reported is the TOTAL memory allocated by the program.
    # So not only allocated by the algorithm. However, they are pretty accurate with high values of m.

    def __init__(self):
        super().__init__(metric_1='memory', profiler=False, y_label='Used memory (bytes)')


    def get_stream(self, iteration, m, N):
        return streams.Zipf(1.1, self.generate_seed(), save=False)


    def get_metric_1(self, instance, stream):
        return instance.get_stats()['memory_usage']


class TestAsymptoticTimeProfiler(TestAsymptotic):
    # Computes the "cost" of the algorithm. Using valgrind, it runs in a virtual machine, and only computes the cost
    # of the algorithm, so the rest of the program (I/O mainly) doesn't affect. However, the cost reported is not exactly time,
    # it's the Valgrind's output which is a sum of instructions + cache fails, etc. It's pretty good to compare algorithms but
    # is really slow because of the virtualization.

    def __init__(self):
        super().__init__(metric_1='time', profiler=True, y_label='Cost (Valgrind output)')


    def get_stream(self, iteration, m, N):
        # return streams.Uniform(int(N/2), seed, False)                 # To test the "insert_element" operation (with a few updates)
        # return streams.Uniform(int(1.1*m), seed, False)               # To test the "update_element" operation (with a few inserts)
        return streams.Uniform(2*m, self.generate_seed(), save=False)   # In expectation there will be N/2 inserts and N/2 updates.


    def get_metric_1(self, instance, stream):
        return instance.get_stats()['process_element_cost_profiler'] / stream.N


class TestAsymptoticTime(TestAsymptotic):
    # It's much faster than the profiler version, but with much less accurate results, and it's greatly influenced by more than one executable
    # being executed at the same time. It's better the profiler version.

    def __init__(self):
        super().__init__(metric_1='time', profiler=False, y_label='Mean exec time per element (microseconds)')


    def get_stream(self, iteration, m, N):
        return streams.Uniform(2*m, self.generate_seed(), save=False)


    def get_metric_1(self, instance, stream):
        return instance.get_stats()['process_element_time'] / stream.N


class TestAsymptoticTimeMemory(TestAsymptotic):
    # Fusion of exec time and memory tests in one

    def __init__(self):
        super().__init__(metric_1='time', metric_2='memory', profiler=True, y_label='Cost (Valgrind output)', y_right_label='Used memory (bytes)')


    def get_stream(self, iteration, m, N):
        return streams.Uniform(2*m, self.generate_seed(), save=False)


    def get_metric_1(self, instance, stream):
        return instance.get_stats()['process_element_cost_profiler'] / stream.N


    def get_metric_2(self, instance, stream):
        return instance.get_stats()['memory_usage']


class TestAsymptoticAccuracy(TestAsymptotic):
    # Test to inspect how the precision and recall metrics variate when increasing the m

    def __init__(self):
        super().__init__(metric_1='recall', metric_2='precision', profiler=False, y_label='Accuracy', y_right_label='')
        if self.params.k is not None:
            self.query_param = int(self.params.k)
            self.query_name = 'k_top_query'
        elif self.params.freq is not None:
            self.query_param = int(self.params.freq)
            self.query_name = 'frequent_query'
        else:
            exit(1)


    def get_stream(self, iteration, m, N):
        self.iteration = iteration
        return streams.Zipf(1.001, self.generate_seed(), save=True)


    def get_metric_1(self, instance, stream):
        return metrics.get_recall(instance, stream, self.query_name, self.query_param)


    def get_metric_2(self, instance, stream):
        return metrics.get_precision(instance, stream, self.query_name, self.query_param)



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

