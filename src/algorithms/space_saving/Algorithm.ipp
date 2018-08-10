#include <iostream>

namespace SpaceSaving {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    this->set_monitored_size(m);
}

template<class T>
void Algorithm<T>::frequent_query(float f, ostream& stream) {
    for(auto i = stream_summary.begin(); i != stream_summary.end() && i->count >= f * this->N; ++i) {
        for(auto j = i->elements.begin(); j != i->elements.end(); ++j) {
            stream << (*j)->id << " " << i->count / (float) this->N << endl;
        }
    }
}

template<class T>
void Algorithm<T>::k_top_query(int k, std::ostream& stream) {
    for(auto i = stream_summary.begin(); i != stream_summary.end(); ++i) {
        for(auto j = i->elements.begin(); j != i->elements.end(); ++j) {
            if(k-- <= 0) {
                return;
            }
            stream << (*j)->id << " " << i->count / (float) this->N << endl;
        }
    }
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {

    if(this->sample_size() < m) {
        if(stream_summary.empty() || prev(stream_summary.end())->count != 1) {
            // There are no buckets or the smallest one has count greater than 1
            stream_summary.push_back(typename Element<T>::Bucket(1));
        }
        element.bucket_iterator = prev(stream_summary.end());
        element.bucket_iterator->elements.emplace_back(&element);
        element.element_iterator = prev(element.bucket_iterator->elements.end());
        element.over_estimation = 0;
    } else { // Max number of monitored elements is reached. This new one will replace the one with less hits
        element.bucket_iterator = prev(stream_summary.end());
        element.element_iterator = element.bucket_iterator->elements.begin(); // We select the oldest element with less hits
        T removed_id = (*element.element_iterator)->id;
        this->remove_element(removed_id);

        // Replacing the old element
        *element.element_iterator = &element;
        element.over_estimation = element.bucket_iterator->count;

        increment_counter(element);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    increment_counter(element);
}

template<class T>
void Algorithm<T>::increment_counter(Element<T>& element) {
    int new_count = element.bucket_iterator->count + 1;
    if(element.bucket_iterator == stream_summary.begin() || prev(element.bucket_iterator)->count != new_count) { // It is the highest bucket or the next bucket doesn't have the required count
        if(element.bucket_iterator->elements.size() == 1) { // We can reuse the current bucket
            element.bucket_iterator->count++;
            return;
        } else { // A new bucket needs to be created since there are other elements in the current bucket
            stream_summary.insert(element.bucket_iterator, typename Element<T>::Bucket(new_count));
        }
    }
    element.bucket_iterator->elements.erase(element.element_iterator);
    typename Element<T>::StreamSummary::iterator old_bucket = element.bucket_iterator;
    element.bucket_iterator--;
    if(old_bucket->elements.empty()) {
        stream_summary.erase(old_bucket);
    }
    element.bucket_iterator->elements.push_back(&element);
    element.element_iterator = prev(element.bucket_iterator->elements.end());
}

template<class T>
void Algorithm<T>::print_state() const {
    int n_elements = 0;
    for(auto i = stream_summary.begin(); i != stream_summary.end(); ++i) {
        cout << "-----------------------" << endl;
        cout << "%%%%%% " << i->count << " %%%%%%" << endl;
        for(auto j = i->elements.begin(); j != i->elements.end(); ++j) {
            cout << (*j)->id << ", " << (*j)->over_estimation << endl;
            ++n_elements;
        }
    }
    assert(n_elements == this->sample_size());
}


}