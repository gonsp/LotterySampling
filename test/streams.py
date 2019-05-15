import itertools
from test.sorted_list import SortedList
from abc import abstractmethod
from io import TextIOWrapper

import math
import numpy as np


class Stream():

    def __init__(self, length, save=True):
        self.length = length
        self.N = 0
        self.n = 0
        self.save = save
        self.sorted_list = SortedList()


    def __iter__(self):
        return self


    def __next__(self):
        self.N += 1
        if self.N > self.length:
            raise StopIteration
        element = self.next_element()
        if self.save:  # To speed-up tests in which it is not necessary to check accuracy
            self.sorted_list.process_element(element)
            self.n = self.sorted_list.size()
        return element


    @abstractmethod
    def next_element(self):
        pass


    def top_k_query(self, k):
        return [(id, count/self.N) for id, count in itertools.islice(iter(self.sorted_list), k)]


    def frequent_query(self, freq):
        return [(id, count/self.N) for id, count in itertools.takewhile(lambda element: element[1] >= freq * self.N, iter(self.sorted_list))]


class Zipf(Stream):

    def __init__(self, length, alpha=1.5, offset=-1, seed=None, save=True):
        super().__init__(length, save)
        self.alpha = alpha
        self.offset = offset
        np.random.seed(seed)


    def next_element(self):
        element = np.random.zipf(self.alpha)
        while element < self.offset:
            element = np.random.zipf(self.alpha)
        return str(element)


class Uniform(Stream):

    def __init__(self, length, max=100000, seed=None, save=True):
        super().__init__(length, save)
        self.max = max
        np.random.seed(seed)


    def next_element(self):
        return str(np.random.uniform(0, self.max))


class Unequal(Stream):

    def __init__(self, length, alpha=100, beta=100, seed=None, save=True):
        super().__init__(length, save)
        self.data = np.zeros(length, dtype=int)
        for i in range(alpha):
            for j in range(beta):
                self.data[i*beta + j] = i
        for i in range(alpha * beta, length):
            self.data[i] = i - alpha * (beta - 1)
        np.random.seed(seed)
        self.data = np.random.permutation(self.data)


    def next_element(self):
        element = str(self.data[self.N])
        return element


class File(Stream):

    def __init__(self, file_path, length=math.inf, shuffle=False, repetitions=1, seed=None, save=True):
        if shuffle or repetitions > 1:
            self.data = []
            with open(file_path, 'r') as file:
                for line in file:
                    element = line[:-1]
                    self.data.append(element)
            self.data *= repetitions
            if length == -1 or length > len(self.data):
                length = len(self.data)
            if shuffle:
                np.random.seed(seed)
                self.data = np.random.permutation(self.data)
        else:
            self.data = open(file_path, 'r')
        super().__init__(length, save)


    def next_element(self):
        if isinstance(self.data, TextIOWrapper):
            element = self.data.readline()[:-1]
            if element == '':
                raise StopIteration
            return element
        else:
            return self.data[self.N]