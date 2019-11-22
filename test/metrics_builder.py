import os
import numpy as np
from datetime import datetime
import accuracy_metrics


class MetricsBuilder:

    def __init__(self, x_name, metrics):
        self.x = []
        self.y = dict([(metric, []) for metric in [(metric,) if type(metric) != list else tuple(metric) for metric in metrics]])
        self.start_time = datetime.now()
        self.x_name = x_name


    def capture(self, x_value, instances, stream, config):
        for metric_group, data_lines in self.y.items():

            index = 0
            def add_value(line_name, value):
                nonlocal index
                if len(self.x) == 0:
                    data_lines.append((line_name, []))
                data_lines[index][1].append(value)
                index += 1

            for metric in metric_group:
                if metric == "f(x_k)":
                    assert(config["query"] == "top_k")
                    add_value(metric, stream.top_k_query(config["query_param"])[-1][1] * stream.N)
                elif metric == "E[T_k]":
                    assert(config["query"] == "top_k")
                    add_value(metric, 1 - (1 / (stream.top_k_query(config["query_param"])[-1][1] * stream.N + 1)))
                else:
                    for i, instance in enumerate(instances):
                        algorithm = config["algorithms"][i]
                        name = algorithm["name_csv"] if "name_csv" in algorithm else algorithm["name"]
                        if "get_" + metric in dir(accuracy_metrics):
                            add_value(name, getattr(accuracy_metrics, "get_" + metric)(instance, stream, config["query"] + "_query", config["query_param"]))
                        else:
                            stats = instance.get_stats()
                            if metric in stats:
                                add_value(name, instance.get_stats()[metric])
        self.x.append(x_value)


    def save(self, config_json, finished=True):
        folder = "results/" + self.start_time.strftime('%Y-%m-%d-%H:%M:%S') + "/"
        os.makedirs(folder, exist_ok=True)

        for metric_group, data_lines in self.y.items():
            filename = folder + metric_group[0] + ".csv"
            table = np.flip(np.rot90(np.array([self.x] + [data for _, data in data_lines]), axes=(1, 0)), axis=1)
            np.savetxt(filename, table, header=','.join([self.x_name] + [name for name, _ in data_lines]), comments='', delimiter=',')

        unfinished_experiment_config_file = folder + "unfinished_config_file.json"
        finished_experiment_config_file = folder + "config_file.json"
        with open(finished_experiment_config_file if finished else unfinished_experiment_config_file, "w") as config_file:
            config_file.writelines(config_json)
        if os.path.isfile(unfinished_experiment_config_file) and finished:
            os.remove(unfinished_experiment_config_file)
