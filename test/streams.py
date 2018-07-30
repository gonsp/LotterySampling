import numpy as np

class Stream():

    def __init__(self):
        self.N = 0
        self.elements = {}


    def next_element(self, element):
        self.N += 1
        if element in self.elements:
            self.elements[element] += 1
        else:
            self.elements[element] = 1
        self.n = len(self.elements)


class Zipf(Stream):

    def __init__(self, alpha=1.5):
        super().__init__()
        self.alpha = alpha


    def next_element(self):
        element = np.random.zipf(self.alpha)
        super().next_element(element)
        return element
