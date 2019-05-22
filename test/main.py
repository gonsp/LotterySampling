#!/usr/bin/env python
import sys
import atexit
import shutil
from experiment import Experiment


def main():
    atexit.register(lambda: shutil.rmtree(".tmp/", ignore_errors=True))

    config_file_path = sys.argv[1]
    experiment = Experiment(config_file_path)
    experiment.run()

if __name__ == '__main__':
    main()