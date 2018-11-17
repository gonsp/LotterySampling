import heapq
from abc import abstractmethod
import matplotlib.pyplot as plt
import numpy as np

class Stream():

    def __init__(self, N_total, save=True):
        self.N_total = N_total
        self.N = 0
        self.n = 0
        self.save = save
        self.elements = {}
        self.cache_k = None
        self.cache_freq = None
        self.cache_k_param = -1
        self.cache_freq_param = -1


    @abstractmethod
    def next_element(self):
        pass


    def _next_element(self, element):
        self.N += 1
        self.cache_k = None
        self.cache_freq = None
        if self.save:  # To speed-up tests in which is not necessary to check accuracy
            if element in self.elements:
                self.elements[element] += 1
            else:
                self.elements[element] = 1
            self.n = len(self.elements)


    def k_top_query(self, k):
        if self.cache_k is None or self.cache_k_param != k:
            self.cache_k = [(element, self.elements[element]/self.N) for element in heapq.nlargest(k, self.elements, key=self.elements.get)]
            self.cache_k_param = k
        return self.cache_k


    def frequent_query(self, freq):
        if self.cache_freq is None or self.cache_freq_param != freq:
            elements = filter(lambda element: element[1] >= self.N * freq, self.elements.items())
            elements = [(id, count/self.N) for id, count in elements]
            self.cache_freq = sorted(elements, key=lambda element: -element[1])
            self.cache_freq_param = freq
        return self.cache_freq


    def get_name(self):
        return self.name


    def show(self, threshold=100):
        elements = [(int(element), freq) for element, freq in self.elements.items()]
        if threshold is not None:
            elements = filter(lambda element: element[0] < threshold, elements)
        filtered = list(zip(*elements))
        if len(filtered) == 0:
            return
        keys, counters = filtered
        print('Plotted freq sum:', sum(counters) / self.N)
        plt.bar(keys, counters)
        plt.show()


class Zipf(Stream):

    def __init__(self, N_total, alpha=1.5, seed=None, save=True):
        super().__init__(N_total, save)
        self.alpha = alpha
        np.random.seed(seed)
        self.name = 'Zipf,alpha=' + str(alpha) + ',seed=' + str(seed)


    def next_element(self):
        element = np.random.zipf(self.alpha)
        element = str(element)
        super()._next_element(element)
        return element


class Uniform(Stream):

    def __init__(self, N_total, max=100000, seed=None, save=True):
        super().__init__(N_total, save)
        self.max = max
        np.random.seed(seed)
        self.name = 'Uniform,max=' + str(max) + ',seed=' + str(seed)


    def next_element(self):
        element = int(np.random.uniform(0, self.max))
        element = str(element)
        super()._next_element(element)
        return element


class Unequal(Stream):

    def __init__(self, N_total, alpha=100, beta=100, seed=None, save=True):
        super().__init__(N_total, save)
        self.data = np.zeros(N_total, dtype=int)
        for i in range(alpha):
            for j in range(beta):
                self.data[i*beta + j] = i
        for i in range(alpha * beta, N_total):
            self.data[i] = i - alpha * (beta - 1)
        np.random.seed(seed)
        self.data = np.random.permutation(self.data)
        self.name = 'Unequal,alpha=' + str(alpha) + ',beta=' + str(beta) + ',N=' + str(N_total) + ',seed=' + str(seed)


    def next_element(self):
        element = str(self.data[self.N])
        super()._next_element(element)
        return element


class MultiZipf(Stream):

    def __init__(self, N_total, alphas, seed=None, save=True):
        super().__init__(N_total, save)
        self.substreams = []
        for alpha in alphas:
            self.substreams.append(Zipf(alpha, seed=seed, save=False))
        self.name = 'MultiZipf,alphas=' + str(alphas) + ',N=' + str(N_total) + ',seed=' + str(seed)


    def next_element(self):
        i = self.N // (self.N_total // len(self.substreams)) % len(self.substreams)
        element = self.substreams[i].next_element()
        element = str(int(element) + 1000000 * (i + 1))
        super()._next_element(element)
        return element


class File(Stream):

    def __init__(self, N_total, file_path, shuffle=True, repetitions=1, seed=None):
        self.data = []
        with open(file_path, 'r') as file:
            for line in file:
                element = line[:-1]
                self.data.append(element)
        self.data *= repetitions
        if shuffle:
            np.random.seed(seed)
            self.data = np.random.permutation(self.data)
        if N_total == -1 or N_total > len(self.data):
            N_total = len(self.data)
        super().__init__(N_total, True)
        self.name = file_path.split('/')[-1] + ',shuffle=' + str(shuffle) + ',repetitions=' + str(repetitions) + ',N=' + str(N_total)


    def next_element(self):
        element = self.data[self.N]
        super()._next_element(element)
        return element