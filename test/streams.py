import heapq
from abc import abstractmethod
import matplotlib.pyplot as plt
import numpy as np

class Stream():

    def __init__(self, save=True):
        self.N = 0
        self.n = 0
        self.save = save
        self.elements = {}


    @abstractmethod
    def next_element(self):
        pass


    def _next_element(self, element):
        self.N += 1
        if self.save:  # To speed-up tests in which is not necessary to check accuracy
            if element in self.elements:
                self.elements[element] += 1
            else:
                self.elements[element] = 1
            self.n = len(self.elements)


    def k_top_query(self, k):
        return [(element, self.elements[element]/self.N) for element in heapq.nlargest(k, self.elements, key=self.elements.get)]


    def frequent_query(self, freq):
        pass


    @abstractmethod
    def get_name(self):
        pass


    def show(self, threshold=100):
        elements = [(int(element), freq) for element, freq in self.elements.items()]
        if threshold is not None:
            elements = filter(lambda element: element[0] < threshold, elements)
        keys, counters = list(zip(*elements))
        print('Plotted freq sum:', sum(counters) / self.N)
        plt.bar(keys, counters)
        plt.show()


class Zipf(Stream):

    def __init__(self, alpha=1.5, seed=None, save=True):
        super().__init__(save)
        self.alpha = alpha
        np.random.seed(seed)
        self.name = 'Zipf,alpha=' + str(alpha) + ',seed=' + str(seed)


    def next_element(self):
        element = np.random.zipf(self.alpha)
        element = str(element)
        super()._next_element(element)
        return element


    def get_name(self):
        return self.name


class Uniform(Stream):

    def __init__(self, max=100000, seed=None, save=True):
        super().__init__(save)
        self.max = max
        np.random.seed(seed)
        self.name = 'Uniform,max=' + str(max) + ',seed=' + str(seed)


    def next_element(self):
        element = int(np.random.uniform(0, self.max))
        element = str(element)
        super()._next_element(element)
        return element


    def get_name(self):
        return self.name


class Unequal(Stream):

    def __init__(self, alpha=100, beta=100, N=1000000, seed=None, save=True):
        super().__init__(save)
        self.data = np.zeros(N, dtype=int)
        for i in range(alpha):
            for j in range(beta):
                self.data[i*beta + j] = i
        for i in range(alpha * beta, N):
            self.data[i] = i - alpha * (beta - 1)
        np.random.seed(seed)
        self.data = np.random.permutation(self.data)
        self.name = 'Unequal,alpha=' + str(alpha) + ',beta=' + str(beta) + ',N=' + str(N) + ',seed=' + str(seed)


    def next_element(self):
        element = str(self.data[self.N])
        super()._next_element(element)
        return element


    def get_name(self):
        return self.name
