import subprocess
import ast

class Instance():

    def __init__(self, exec_path, params):
        command = [exec_path] + params.split()
        self.process = subprocess.Popen(command, bufsize=1, universal_newlines=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)


    def process_element(self, element):
        self.process.stdin.write(element + '\n')


    def process_query_output(self):
        elements = []
        while (True):
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
        self.process.stdin.write(':s\n')
        output = self.process.stdout.readline()
        return ast.literal_eval(output)



