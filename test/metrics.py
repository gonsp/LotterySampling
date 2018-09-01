import streams
import instance

def get_squared_error(instance, stream, query_name, parameter):
    reported_elements, real_elements = execute_query(instance, stream, query_name, parameter)
    error = 0
    for element in reported_elements.keys() & real_elements.keys():
        error += (reported_elements[element] - real_elements[element])**2
    for element in reported_elements.keys() ^ real_elements.keys():
        if element in reported_elements:
            error += (reported_elements[element] - stream.elements[element]/stream.N)**2
        else:
            error += real_elements[element]**2
    return error


def get_recall(instance, stream, query_name, parameter):
    reported_elements, real_elements = execute_query(instance, stream, query_name, parameter)
    return len(reported_elements.keys() & real_elements.keys()) / len(real_elements)


def get_precision(instance, stream, query_name, parameter):
    reported_elements, real_elements = execute_query(instance, stream, query_name, parameter)
    return len(reported_elements.keys() & real_elements.keys()) / len(reported_elements)


def get_weighted_recall(instance, stream, query_name, parameter):
    reported_elements, real_elements = execute_query(instance, stream, query_name, parameter)
    return get_intersection_sum(reported_elements, real_elements) / sum(freq for freq in real_elements.values())


def get_weighted_precision(instance, stream, query_name, parameter):
    reported_elements, real_elements = execute_query(instance, stream, query_name, parameter)
    for element in reported_elements:
        reported_elements[element] = 1 - reported_elements[element]
    for element in real_elements:
        real_elements[element] = 1 - real_elements[element]
    real_freq_sum_reported_elements = sum([1 - stream.elements[element] / stream.N for element in reported_elements.keys()])
    return get_intersection_sum(reported_elements, real_elements) / real_freq_sum_reported_elements


def get_weighted_precision_2(instance, stream, query_name, parameter):
    reported_elements, real_elements = execute_query(instance, stream, query_name, parameter)
    real_freq_sum_reported_elements = sum([stream.elements[element] / stream.N for element in reported_elements.keys()])
    return real_freq_sum_reported_elements / sum(freq for freq in real_elements.values())


def execute_query(instance, stream, query_name, parameter):
    query = getattr(instance, query_name)
    reported_elements = dict(query(parameter))
    query = getattr(stream, query_name)
    real_elements = dict(query(parameter))
    return reported_elements, real_elements


def get_intersection_sum(reported_elements, real_elements):
    return sum([real_elements[element] for element in reported_elements.keys() & real_elements.keys()])