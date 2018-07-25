#include "SpaceSaving.h"

SpaceSaving::SpaceSaving(const InputParser& parameters) {
    m = stoi(parameters.get_parameter("-m"));
    // TODO experiment with different max_load_factor
    monitored_elements.max_load_factor(1);
    monitored_elements.reserve(m);
}

void SpaceSaving::frequent_query(std::ostream& stream) {

}

void SpaceSaving::k_top_query(std::ostream& stream) {

}

void SpaceSaving::process_element(std::string& element_id) {
    MonitoredElements::iterator it = monitored_elements.find(element_id);
    if(it == monitored_elements.end()) { // element wasn't being sampled
        if(monitored_elements.size() < m) {
            if(stream_summary.empty() || prev(stream_summary.end())->count != 1) { // There are no buckets or the smaller one has count greater than 0
                stream_summary.push_back(Bucket(1));
            }
            ElementLocator locator;
            locator.bucket_iterator = prev(stream_summary.end());
            locator.bucket_iterator->elements.emplace_back(element_id, 0);
            locator.element_iterator = prev(locator.bucket_iterator->elements.end());
            monitored_elements[element_id] = locator;
        } else { // Max number of monitored elements is reached. This new one will replace the one with less hits

        }
    } else { // element was being sampled
        ElementLocator& locator = it->second;
        Element element = *locator.element_iterator;
        int new_count = locator.bucket_iterator->count + 1;
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
}
