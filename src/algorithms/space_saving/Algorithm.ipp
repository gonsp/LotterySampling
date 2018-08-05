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
bool Algorithm<T>::insert_element(const T& element_id, Locator& locator) {
    locator = make_shared<Element<T>>(element_id, 0);

    if(this->sample_size() < m) {
        if(stream_summary.empty() || prev(stream_summary.end())->count != 1) {
            // There are no buckets or the smallest one has count greater than 1
            stream_summary.push_back(Bucket<T>(1));
        }
        locator->bucket_iterator = prev(stream_summary.end());
        locator->bucket_iterator->elements.emplace_back(locator);
        locator->element_iterator = prev(locator->bucket_iterator->elements.end());
    } else { // Max number of monitored elements is reached. This new one will replace the one with less hits
        locator->bucket_iterator = prev(stream_summary.end());
        locator->element_iterator = locator->bucket_iterator->elements.begin(); // We select the oldest element with less hits
        T removed_id = (*locator->element_iterator)->id;
        this->remove_element(removed_id);

        // Replacing the old element
        *locator->element_iterator = locator;
        locator->over_estimation = locator->bucket_iterator->count - 1;

        increment_counter(locator);
    }
    return true;
}

template<class T>
void Algorithm<T>::update_element(Locator& locator) {
    increment_counter(locator);
}

template<class T>
void Algorithm<T>::increment_counter(Locator& locator) {
    int new_count = locator->bucket_iterator->count + 1;
    locator->bucket_iterator->elements.erase(locator->element_iterator);
    if(locator->bucket_iterator == stream_summary.begin() || prev(locator->bucket_iterator)->count != new_count) { // It is the highest bucket or the previous bucket doesn't have the required count
        stream_summary.insert(locator->bucket_iterator, Bucket<T>(new_count));
    }
    typename Element<T>::BucketIterator old_bucket = locator->bucket_iterator;
    locator->bucket_iterator--;
    if(old_bucket->elements.empty()) {
        stream_summary.erase(old_bucket);
    }
    locator->bucket_iterator->elements.push_back(locator);
    locator->element_iterator = prev(locator->bucket_iterator->elements.end());
}

template<class T>
void Algorithm<T>::print_state() {
    int n_elements = 0;
    for(auto i = stream_summary.begin(); i != stream_summary.end(); ++i) {
        cout << "-----------------------" << endl;
        cout << "%%%%%% " << i->count << " %%%%%%" << endl;
        for(auto j = i->elements.begin(); j != i->elements.end(); ++j) {
            cout << (*j)->id << ", " << (*j)->over_estimation << endl;
            assert(i == this->get_locator((*j)->id).bucket_iterator);
            assert(j == this->get_locator((*j)->id).element_iterator);
            ++n_elements;
        }
    }
    assert(n_elements == this->sample_size());
}


}