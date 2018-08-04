import subprocess
import ast
import time
import profiler_utils

class Instance():

    def __init__(self, exec_path, params, profile=None):
        self.name = params[1:]
        command = [exec_path] + params.split()
        error_pipe = None
        if profile is not None:
            if profile is 'memory_usage':
                tool = 'massif'
            elif profile is 'memory_leak':
                tool = 'memcheck'
            elif profile is 'exec_time':
                tool = 'callgrind'
            else:
                exit(1)
            command = ['valgrind', '--tool=' + tool] + command
            error_pipe = subprocess.PIPE
        self.profile = profile
        self.command = ' '.join(command)
        self.process = subprocess.Popen(command, bufsize=1, universal_newlines=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=error_pipe)
        self.pid = self.process.pid
        self.finished = False


    def process_element(self, element):
        self.process.stdin.write(element + '\n')


    def process_query_output(self):
        elements = []
        while True:
            output = self.process.stdout.readline()
            if output == ':end\n':
                break
            else:
                output = output.split()
                element = (output[0], float(output[1]))
                elements.append(element)
        return elements


    def frequent_query(self, freq):
        command = ':q' + '\n' + ':f' + '\n' + str(freq) + '\n'
        self.process.stdin.write(command)

        return self.process_query_output()


    def k_top_query(self, k):
        command = ':q' + '\n' + ':k' + '\n' + str(k) + '\n'
        self.process.stdin.write(command)

        return self.process_query_output()


    def get_stats(self):
        if self.process.poll() is not None or self.finished:
            return self.end_stats
        self.process.stdin.write(':s\n')
        output = self.process.stdout.readline()
        return ast.literal_eval(output)


    def print_state(self):
        self.process.stdin.write(':d\n')
        while True:
            line = self.process.stdout.readline()
            if line == ':end\n':
                break
            else:
               print(line)


    def finish(self):
        self.end_stats = self.get_stats()
        self.process.stdin.close()
        self.finished = True
        time.sleep(2)
        if self.profile is 'memory_usage':
            self.end_stats['memory_usage_peak_profiler'] = profiler_utils.get_peak_memory(self.pid)
        elif self.profile is 'memory_leak':
            self.end_stats['memory_leak_profiler'] = profiler_utils.get_leak_memory(self.process.stderr)
        elif self.profile is 'exec_time':
            cost_total, cost_process_element = profiler_utils.get_cost(self.pid)
            self.end_stats['total_cost_profiler'] = cost_total
            self.end_stats['process_element_cost_profiler'] = cost_process_element