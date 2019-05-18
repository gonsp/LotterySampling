class Bucket:

    def __init__(self, freq):
        self.freq = freq
        self.elements = set()
        self.prev_bucket = None
        self.next_bucket = None


class SortedList:

    def __init__(self):
        self.element_to_bucket = {}
        self.first_bucket = None
        self.last_bucket = None


    def __getitem__(self, id):
        return self.element_to_bucket[id].freq


    def __iter__(self):
        bucket = self.first_bucket
        while bucket is not None:
            for element in bucket.elements:
                yield (element, bucket.freq)
            bucket = bucket.next_bucket


    def process_element(self, id):
        if id not in self.element_to_bucket:
            self.add(id)
        else:
            self.increase_freq(id)


    def add(self, id):
        if self.last_bucket is None or self.last_bucket.freq != 1:
            bucket = Bucket(1)
            if self.last_bucket is None:
                self.first_bucket = bucket
            else:
                bucket.prev_bucket = self.last_bucket
                self.last_bucket.next_bucket = bucket
            self.last_bucket = bucket
        else:
            bucket = self.last_bucket

        bucket.elements.add(id)
        self.element_to_bucket[id] = bucket


    def increase_freq(self, id):
        current_bucket = self.element_to_bucket[id]
        current_bucket.elements.remove(id)

        prev_bucket = current_bucket.prev_bucket
        if prev_bucket is None:
            prev_bucket = Bucket(current_bucket.freq + 1)
            self.first_bucket = prev_bucket
            current_bucket.prev_bucket = prev_bucket
            prev_bucket.next_bucket = current_bucket
            new_bucket = prev_bucket
        elif prev_bucket.freq != current_bucket.freq + 1:
            new_bucket = Bucket(current_bucket.freq + 1)
            new_bucket.prev_bucket = prev_bucket
            new_bucket.next_bucket = current_bucket
            prev_bucket.next_bucket = new_bucket
            current_bucket.prev_bucket = new_bucket
        else:
            new_bucket = prev_bucket

        new_bucket.elements.add(id)
        self.element_to_bucket[id] = new_bucket

        if len(current_bucket.elements) == 0:
            if self.last_bucket == current_bucket:
                self.last_bucket = new_bucket
            else:
                current_bucket.next_bucket.prev_bucket = current_bucket.prev_bucket
            current_bucket.prev_bucket.next_bucket = current_bucket.next_bucket
            del current_bucket


    def size(self):
        return len(self.element_to_bucket)
