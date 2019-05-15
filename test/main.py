#!/usr/bin/env python
import sys
from experiment import Experiment


def main():
    config_file_path = sys.argv[1]
    experiment = Experiment(config_file_path)
    experiment.run()

if __name__ == '__main__':
    main()