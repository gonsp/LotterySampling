import os


def get_peak_memory(pid):
    process_element_line = None
    instantiation_lines = []
    with open('../src/Main.cpp') as f:
        for line_number, line in enumerate(f):
            if 'algorithm->process_element' in line:
                process_element_line = line_number + 1
            elif 'algorithm = new' in line:
                instantiation_lines.append(line_number + 1)

    massif_file = 'massif.out.' + str(pid)

    peak = 0
    with open(massif_file) as f:
        used_memory = 0
        for line in f:
            if 'snapshot=' in line:
                peak = max(used_memory, peak)
                used_memory = 0
            elif 'Main.cpp:' + str(process_element_line) in line or \
                any(['Main.cpp:' + str(line_number) in line for line_number in instantiation_lines]):
                used_memory += int(line.split()[1])

    peak = max(used_memory, peak)

    os.remove(massif_file)

    return peak


def get_leak_memory(pipe):
    while True:
        line = pipe.readline()
        if 'definitely lost:' in line:
            return int(line.split()[3].replace(',', ''))
    exit(1)


def get_cost(pid):

    callgrind_file = 'callgrind.out.' + str(pid)
    id_main = None
    id_process_element = None

    def line_to_id(line):
        return line.split(' ')[0].split('=')[1].replace('(', '').replace(')', '')

    with open(callgrind_file) as f:
        for line in f:
            if '>::process_element' in line:
                id_process_element = line_to_id(line)
            elif 'Main.cpp' in line:
                line = f.readline()
                id_main = line_to_id(line)

            if id_main is not None and id_process_element is not None:
                break

    if id_main is None or id_process_element is None:
        exit(1)

    cost_main = None
    cost_process_element = None

    def line_to_cost(line):
        return int(line.split(' ')[1])

    with open(callgrind_file) as f:
        for line in f:
            if 'cfn=(' + id_process_element + ')' in line:
                f.readline()
                cost_process_element = line_to_cost(f.readline())
            elif 'cfn=(' + id_main + ')' in line:
                f.readline()
                cost_main = line_to_cost(f.readline())

            if cost_main is not None and cost_process_element is not None:
                break

    os.remove(callgrind_file)

    return cost_main, cost_process_element
