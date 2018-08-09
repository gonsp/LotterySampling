import streams
import instance

def get_weighted_recall(instance, stream, query_name, parameter):
    reported_elements, real_elements = execute_query(instance, stream, query_name, parameter)
    return get_common_elements(reported_elements, real_elements) / sum(freq for _, freq in real_elements)
    return recall


def get__weighted_precision(instance, stream, query_name, parameter):
    reported_elements, real_elements = execute_query(instance, stream, query_name, parameter)
    real_freq_sum_reported_elements = sum([stream.elements[element]/stream.N for element in stream.elements.keys() & list(zip(*reported_elements))[0]])
    return get_common_elements(reported_elements, real_elements) / real_freq_sum_reported_elements
    return precision


def execute_query(instance, stream, query_name, parameter):
    query = getattr(instance, query_name)
    reported_elements = query(parameter)
    query = getattr(stream, query_name)
    real_elements = query(parameter)
    return reported_elements, real_elements


def get_common_elements(reported_elements, real_elements):
    return sum([real_freq for element, real_freq in real_elements if element in list(zip(*reported_elements))[0]])