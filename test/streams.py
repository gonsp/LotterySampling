import numpy as np

class Stream():

    def __init__(self):
        self.N = 0


    def next_element(self):
        self.N += 1


class Zipf(Stream):

    def __init__(self, alpha=1.5):
        super().__init__()
        self.alpha = alpha


    def next_element(self):
        super().next_element()
        return np.random.zipf(self.alpha)
