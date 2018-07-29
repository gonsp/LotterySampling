import subprocess

class Instance():

    def __init__(self, exec_path, params):
        command = [exec_path] + params.split(' ')
        self.process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
