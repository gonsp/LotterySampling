#!/usr/bin/env python
import sys
import json
from test.experiment import Experiment


def main():
    experiment_config_path = sys.argv[1]
    with open(experiment_config_path) as config_file:
        configuration = json.loads(config_file.read())

    experiment = Experiment(configuration)
    experiment.run()

if __name__ == '__main__':
    main()