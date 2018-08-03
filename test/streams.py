import heapq
from abc import abstractmethod

import numpy as np

class Stream():

    def __init__(self):
        self.N = 0
        self.n = 0
        self.elements = {}
        # TODO consider AVL tree implementation for dict for O(k) queries https://pypi.org/project/bintrees/


    @abstractmethod
    def next_element(self):
        pass


    def _next_element(self, element):
        self.N += 1
        if element in self.elements:
            self.elements[element] += 1
        else:
            self.elements[element] = 1
        self.n = len(self.elements)


    def k_top_query(self, k):
        return [(element, self.elements[element]/self.N) for element in heapq.nlargest(k, self.elements, key=self.elements.get)]


    def frequent_query(self, freq):
        pass


class Zipf(Stream):

    def __init__(self, alpha=1.5, seed=None):
        super().__init__()
        self.alpha = alpha
        np.random.seed(seed)


    def next_element(self):
        element = np.random.zipf(self.alpha)
        element = str(element)
        super()._next_element(element)
        return element
