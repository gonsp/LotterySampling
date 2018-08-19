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
        self.test_command = ' '.join(sys.argv[1:])
        self.exec_path = self.build_executable(configuration)
        arg_parser = argparse.ArgumentParser()
        arg_parser.add_argument('test_name')
        for name, required in args:
            arg_parser.add_argument('-' + name, required=required)
        self.params = arg_parser.parse_args()


    @abstractmethod
    def run(self):
        pass


    def create_instances(self, m, seed, profile=None):
        configuration = 'release' if profile is None else 'debug'
        return [
            # Instance('../bin/optimization-1-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile=profile),
            # Instance('../bin/optimization-1-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging -multilevel', profile=profile),
            # Instance('../bin/optimization-1-' + configuration, '-a space_saving -m ' + str(m), profile=profile),
            # Instance('../bin/optimization-2-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile=profile),
            # Instance('../bin/optimization-2-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging -multilevel', profile=profile),
            # Instance('../bin/optimization-2-' + configuration, '-a space_saving -m ' + str(m), profile=profile),
            # Instance('../bin/optimization-3-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile=profile),
            # Instance('../bin/optimization-3-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging -multilevel', profile=profile),
            # Instance('../bin/optimization-3-' + configuration, '-a space_saving -m ' + str(m), profile=profile),
            Instance('../bin/optimization-4-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile=profile),
            # Instance('../bin/optimization-4-' + configuration, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging -multilevel', profile=profile),
            # Instance('../bin/optimization-4-' + configuration, '-a space_saving -m ' + str(m), profile=profile),
            Instance(self.exec_path, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging', profile=profile),
            # Instance(self.exec_path, '-a lottery_sampling -m ' + str(m) + ' -seed ' + str(seed) + ' -aging -multilevel', profile=profile),
            # Instance(self.exec_path, '-a space_saving -m ' + str(m), profile=profile)
        ]


    def build_executable(self, configuration='release'):
        print('BUILDING EXECUTABLE FOR TEST')
        directory = '../' + 'cmake-build-' + configuration
        if not os.path.exists(directory):
            os.makedirs(directory)
        command = 'cd ' + directory + ' && cmake -DCMAKE_BUILD_TYPE=' + configuration + ' .. && make'
        process = subprocess.Popen(command, shell=True)
        process.communicate()
        assert(process.returncode == 0)

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

        m = 10000
        seed = self.generate_seed()
        stream = streams.Uniform(3*m, seed, save=False)
        instances = self.create_instances(m, seed, 'memory_leak')

        for i in range(1000000):
            element = stream.next_element()
            for instance in instances:
                instance.process_element(str(element))

        for instance in instances:
            instance.k_top_query(m/2)
            instance.frequent_query(0.05)

            instance.finish()
            memory_leak = instance.get_stats()['memory_leak_profiler']
            if memory_leak > 0:
                self.error('Memory leak:', memory_leak, 'bytes', seed=seed, command=instance.command)


class TestAsymptotic(Test):
    # To test asymptotic behaviour of memory, exec time or accuracy (and sample size) with respect to the initial sample size

    def __init__(self, profile=None, metrics_left=[], metrics_right=['sample size'], x_label='m', y_left_label='', y_right_label='Num elements'):
        args = [
            ('initial_m', True),
            ('N', True),
            ('iterations', True),
            ('seed', False),
            ('k', False),
            ('freq', False)
        ]
        self.x_label = x_label
        self.y_left_label = y_left_label
        self.y_right_label = y_right_label
        self.metrics_left = metrics_left
        self.metrics_right = metrics_right
        self.profile = profile

        super().__init__(configuration='release' if profile is None else 'debug', args=args)


    def run(self):

        self.seed = self.generate_seed()

        if self.params.iterations is None:
            self.params.iterations = 10
        else:
            self.params.iterations = int(self.params.iterations)

        X = []
        Y_left = []
        Y_right = []
        m_hist = []
        n_hist = []

        for iteration in range(1, self.params.iterations + 1):
            print('Iteration:', iteration, '/', self.params.iterations)

            self.new_iteration(iteration)

            instances = self.create_instances(self.m, self.seed, self.profile)

            for i in range(self.N):
                element = self.stream.next_element()
                for instance in instances:
                    instance.process_element(str(element))

            Y_left_results = []
            Y_right_results = []
            for instance in instances:
                if self.profile is not None:
                    instance.finish()
                Y_left_results.append(self.get_metrics_left(iteration, instance))
                Y_right_results.append(self.get_metrics_right(iteration, instance))
                instance.finish()

            print(Y_left_results)
            print(Y_right_results)

            X.append(self.get_X_value(iteration))
            Y_left.append(Y_left_results)
            Y_right.append(Y_right_results)
            m_hist.append(self.m)
            n_hist.append(self.stream.n)

        X = np.array(X)
        Y_left = np.array(Y_left)
        Y_right = np.array(Y_right)
        m_hist = np.array(m_hist)
        n_hist = np.array(n_hist)

        print('Showing results')
        _, axes_left = plt.subplots()
        axes_right = axes_left.twinx()

        for Y, metrics, axes in [(Y_left, self.metrics_left, axes_left), (Y_right, self.metrics_right, axes_right)]:
            for metric_index in range(0, Y.shape[2]):
                axes.set_prop_cycle(None)
                for i, instance in enumerate(instances):
                    line_format = '-*' if metric_index == 0 else '-.x'
                    line_format = line_format if axes == axes_left else '--'
                    axes.plot(X, Y[:, i, metric_index], line_format, label=metrics[metric_index] + ' ' + instance.name)

        if self.metrics_right == ['sample size']:
            axes_right.plot(X, m_hist * np.log(n_hist/m_hist), 'm:', label='m * ln(n/m)')

        axes_left.legend(loc='upper left')
        axes_right.legend(loc='upper right')

        axes_left.set_xlabel(self.x_label)
        axes_left.set_ylabel(self.y_left_label)
        axes_right.set_ylabel(self.y_right_label)

        self.full_screen_plot()
        plt.title(self.test_command)
        plt.show()


    @abstractmethod
    def new_iteration(self, iteration):
        pass


    def get_X_value(self, iteration):
        return self.m


    @abstractmethod
    def get_metrics_left(self, iteration, instance):
        pass


    def get_metrics_right(self, iteration, instance):
        return [instance.get_stats()['sample_size']]


class TestAsymptoticMemoryProfiler(TestAsymptotic):
    # Tests the total memory allocated ONLY by the algorithm. It gets very accurate results, although maybe the
    # other version suffices to get good enough results since this one is a bit slower.

    def __init__(self):
        super().__init__(profile='memory_usage', metrics_left=['memory'], y_left_label='Max used memory (bytes)')

    def new_iteration(self, iteration):
        self.N = int(self.params.N)
        self.m = iteration * int(self.params.initial_m)
        self.stream = streams.Zipf(1.1, self.generate_seed(), save=True)

    def get_metrics_left(self, iteration, instance):
        return [instance.get_stats()['memory_usage_peak_profiler']]


class TestAsymptoticMemory(TestAsymptotic):
    # Faster than the profiler version but the memory reported is the TOTAL memory allocated by the program.
    # So not only allocated by the algorithm. However, they are pretty accurate with high values of m.

    def __init__(self):
        super().__init__(metrics_left=['memory'], y_left_label='Used memory (bytes)')


    def new_iteration(self, iteration):
        self.N = int(self.params.N)
        self.m = iteration * int(self.params.initial_m)
        self.stream = streams.Zipf(1.1, self.generate_seed(), save=True)

 
    def get_metrics_left(self, iteration, instance):
        return [instance.get_stats()['memory_usage']]


class TestAsymptoticTimeProfiler(TestAsymptotic):
    # Computes the "cost" of the algorithm. Using valgrind, it runs in a virtual machine, and only computes the cost
    # of the algorithm, so the rest of the program (I/O mainly) doesn't affect. However, the cost reported is not exactly time,
    # it's the Valgrind's output which is a sum of instructions + cache fails, etc. It's pretty good to compare algorithms but
    # is really slow because of the virtualization.

    def __init__(self):
        super().__init__(profile='exec_time', metrics_left=['time'], y_left_label='Cost (Valgrind output)')


    def new_iteration(self, iteration):
        self.N = int(self.params.N)
        self.m = iteration * int(self.params.initial_m)
        # self.stream = streams.Uniform(int(N / 2))                                 # To test the "insert_element" operation (with a few updates)
        # self.stream = streams.Uniform(int(1.1 * self.m))                          # To test the "update_element" operation (with a few inserts)
        self.stream = streams.Uniform(2 * self.m, self.generate_seed(), save=True)  # In expectation there will be N/2 inserts and N/2 updates.


    def get_metrics_left(self, iteration, instance):
        return [instance.get_stats()['process_element_cost_profiler'] / self.N]


class TestAsymptoticTime(TestAsymptotic):
    # It's much faster than the profiler version, but with much less accurate results, and it's greatly influenced by more than one executable
    # being executed at the same time. It's better the profiler version.

    def __init__(self):
        super().__init__(metrics_left=['time'], y_left_label='Mean exec time per element (microseconds)')


    def new_iteration(self, iteration):
        self.N = int(self.params.N)
        self.m = iteration * int(self.params.initial_m)
        self.stream = streams.Uniform(2 * self.m, self.generate_seed(), save=True)  # In expectation there will be N/2 inserts and N/2 updates.


    def get_metrics_left(self, iteration, instance):
        return [instance.get_stats()['process_element_time'] / self.N]


class TestAsymptoticTimeMemory(TestAsymptotic):
    # Fusion of exec time and memory tests in one

    def __init__(self):
        super().__init__(profile='exec_time', metrics_left=['time'], metrics_right=['memory'], y_left_label='Cost (Valgrind output)', y_right_label='Used memory (bytes)')


    def new_iteration(self, iteration):
        self.N = int(self.params.N)
        self.m = iteration * int(self.params.initial_m)
        self.stream = streams.Uniform(2 * self.m, self.generate_seed(), save=False) # In expectation there will be N/2 inserts and N/2 updates.


    def get_metrics_left(self, iteration, instance):
        return [instance.get_stats()['process_element_cost_profiler'] / self.N]


    def get_metrics_right(self, iteration, instance):
        return [instance.get_stats()['memory_usage']]


class TestAsymptoticAccuracy(TestAsymptotic):
    # Test to inspect how the precision and recall metrics variate when increasing the m
    
    def __init__(self):
        super().__init__(metrics_left=['recall', 'precision'], metrics_right=['squared_error'], y_left_label='Accuracy', y_right_label='Squared error')
        if self.params.k is not None:
            self.query_param = int(self.params.k)
            self.query_name = 'k_top_query'
        elif self.params.freq is not None:
            self.query_param = int(self.params.freq)
            self.query_name = 'frequent_query'
        else:
            assert False


    def new_iteration(self, iteration):
        self.N = int(self.params.N)
        self.m = iteration * int(self.params.initial_m)
        self.stream = streams.Zipf(1.01, self.generate_seed(), save=True)


    def get_metrics_left(self, iteration, instance):
        return [metrics.get_weighted_recall(instance, self.stream, self.query_name, self.query_param),
                metrics.get_weighted_precision(instance, self.stream, self.query_name, self.query_param)]


    def get_metrics_right(self, iteration, instance):
        return [metrics.get_squared_error(instance, self.stream, self.query_name, self.query_param)]
