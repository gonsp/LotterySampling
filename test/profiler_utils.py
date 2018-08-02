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
