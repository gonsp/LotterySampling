#include "Algorithm.h"
#include <iostream>

using namespace SpaceSaving;
using namespace std;

Algorithm::Algorithm(const InputParser& parameters) {
    m = (unsigned  int) stoul(parameters.get_parameter("-m"));
    // TODO experiment with different max_load_factor
    monitored_elements.max_load_factor(1);
    monitored_elements.reserve(m);
}

void Algorithm::frequent_query(float f, ostream& stream) {
    for(auto i = stream_summary.begin(); i != stream_summary.end() && i->count >= f*N; ++i) {
        for(auto j = i->elements.begin(); j != i->elements.end(); ++j) {
            stream << j->id << " " << i->count / (float) N << endl;
        }
    }
}

void Algorithm::k_top_query(int k, std::ostream& stream) {
    for(auto i = stream_summary.begin(); i != stream_summary.end(); ++i) {
        for(auto j = i->elements.begin(); j != i->elements.end(); ++j) {
            if(k-- <= 0) {
                return;
            }
            stream << j->id << " " << i->count / (float) N << endl;
        }
    }
}

ElementLocator Algorithm::insert_element(std::string& element_id) {
    ElementLocator locator;
    if(monitored_elements.size() < m) {
        if(stream_summary.empty() || prev(stream_summary.end())->count != 1) {
            // There are no buckets or the smallest one has count greater than 1
            stream_summary.push_back(Bucket(1));
        }
        locator.bucket_iterator = prev(stream_summary.end());
        locator.bucket_iterator->elements.emplace_back(element_id, 0);
        locator.element_iterator = prev(locator.bucket_iterator->elements.end());
    } else { // Max number of monitored elements is reached. This new one will replace the one with less hits
        locator.bucket_iterator = prev(stream_summary.end());
        locator.element_iterator = locator.bucket_iterator->elements.begin(); // We select the oldest element with less hits
        string removed_id = locator.element_iterator->id;
        monitored_elements.erase(removed_id);

        // Replacing the old element
        locator.element_iterator->id = element_id;
        locator.element_iterator->over_estimation = locator.bucket_iterator->count;

        increment_counter(locator);
    }
    return locator;
}

void Algorithm::update_element(SpaceSaving::ElementLocator& locator) {
    increment_counter(locator);
}

void Algorithm::increment_counter(ElementLocator& locator) {
    int new_count = locator.bucket_iterator->count + 1;
    Element element = *locator.element_iterator;
    locator.bucket_iterator->elements.erase(locator.element_iterator);
    if(locator.bucket_iterator == stream_summary.begin() || prev(locator.bucket_iterator)->count != new_count) { // It is the highest bucket or the previous bucket doesn't have the required count
        stream_summary.insert(locator.bucket_iterator, Bucket(new_count));
    }
    StreamSummary::iterator aux_it = locator.bucket_iterator;
    locator.bucket_iterator--;
    if(aux_it->elements.empty()) {
        stream_summary.erase(aux_it);
    }
    locator.bucket_iterator->elements.push_back(element);
    locator.element_iterator = prev(locator.bucket_iterator->elements.end());
}

void Algorithm::print_state() {
    int n_elements = 0;
    for(auto i = stream_summary.begin(); i != stream_summary.end(); ++i) {
        cout << "-----------------------" << endl;
        cout << "%%%%%% " << i->count << " %%%%%%" << endl;
        for(auto j = i->elements.begin(); j != i->elements.end(); ++j) {
            cout << j->id << ", " << j->over_estimation << endl;
            assert(i == monitored_elements[j->id].bucket_iterator);
            assert(j == monitored_elements[j->id].element_iterator);
            ++n_elements;
        }
    }
    assert(n_elements == monitored_elements.size());
}