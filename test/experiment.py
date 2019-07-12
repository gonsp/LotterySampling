import json
import copy
from streams import chunk_stream
from instance import Instance
from metrics_builder import MetricsBuilder


class Experiment:

    def __init__(self, config_file_path):
        self.config, self.config_json = self.load_config_file(config_file_path)

        self.iterating_over = None
        self.iterations = 1
        for i, algorithm in enumerate(self.config["algorithms"]):
            for name, value in algorithm["params"].items():
                if isinstance(value, list):
                    if self.iterating_over is None:
                        assert(i == 0)
                        self.iterating_over = ("algorithms", name)
                        self.iterations = len(value)
                    elif self.iterating_over[1] != name:
                        print("Iterating over more than one parameter is not possible.")
                        exit(1)
                elif self.iterating_over is not None and self.iterating_over[1] == name:
                    print("All the algorithms must have a list in the parameter iterated over.")
                    exit(1)

        for name, value in self.config["stream"]["params"].items():
            if isinstance(value, list):
                if self.iterating_over is None:
                    self.iterating_over = ("stream", name)
                    self.iterations = len(value)
                else:
                    print("Iterating over more than one parameter is not possible.")
                    exit(1)

        self.profile = None
        for metric in self.config["metrics"]:
            if metric in ["memory_usage_profiler", "memory_leak_profiler", "average_cost_profiler"]:
                assert(self.profile is None)
                assert(self.iterating_over is not None)
                self.profile = metric
            else:
                self.config["stream"]["params"]["save"] = True
        if "save" not in self.config["stream"]["params"]:
            self.config["stream"]["params"]["save"] = False


    def load_config_file(self, config_file_path):
        with open(config_file_path) as config_file:
            config_json = config_file.readlines()
        config_json_without_comments = ""
        for line in config_json:
            if "//" not in line:
                config_json_without_comments += line
        return json.loads(config_json_without_comments), config_json


    def get_random_seed(self):
        return self.config["seed"] if "seed" in self.config else random.randrange(10000000)


    def create_instances(self, iteration):
        print("Building binaries and instantiating algorithms")
        instances = []
        for algorithm in self.config["algorithms"]:
            params = copy.deepcopy(algorithm["params"])
            if self.iterating_over is not None and self.iterating_over[0] == "algorithms":
                params[self.iterating_over[1]] = params[self.iterating_over[1]][iteration]
            if "seed" not in params:
                params["seed"] = self.get_random_seed()
            commit = algorithm["commit"] if "commit" in algorithm else None
            instances.append(Instance(algorithm["name"], params, commit=commit, profile=self.profile))
        return instances


    def create_stream(self, iteration):
        stream_name = self.config["stream"]["name"]
        params = copy.deepcopy(self.config["stream"]["params"])
        if self.iterating_over is not None and self.iterating_over[0] == "stream":
            params[self.iterating_over[1]] = params[self.iterating_over[1]][iteration]
        if "seed" not in params:
            params["seed"] = self.get_random_seed()
        mod = __import__('streams', fromlist=[stream_name])
        return getattr(mod, stream_name)(**params)


    def run(self):
        metrics = MetricsBuilder("N" if self.iterating_over is None else self.iterating_over[1], self.config["metrics"])

        for iteration in range(0, self.iterations):
            print('Iteration:', iteration + 1, '/', self.iterations)

            instances = self.create_instances(iteration)
            stream = self.create_stream(iteration)

            for chunk in chunk_stream(stream, stream.length // 100):
                for instance in instances:
                    instance.process_stream_chunk(chunk)
                print(round(stream.N * 100 / stream.length, 2), '%')
                if self.iterating_over is None:
                    metrics.capture(stream.N, instances, stream, self.config)
                    metrics.save(self.config_json, finished=False)

            if self.profile is not None:
                for instance in instances:
                    instance.finish()

            if self.iterating_over is not None:
                if self.iterating_over[0] == "algorithms":
                    x = next(iter(self.config["algorithms"]))["params"][self.iterating_over[1]][iteration]
                else:
                    x = self.config["stream"]["params"][self.iterating_over[1]][iteration]
                metrics.capture(x, instances, stream, self.config)
                metrics.save(self.config_json, finished=False)

            if self.profile is None:
                for instance in instances:
                    instance.finish()

        metrics.save(self.config_json, finished=True)