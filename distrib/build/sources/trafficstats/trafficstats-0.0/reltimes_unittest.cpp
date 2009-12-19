// unit test for the reltimes (and timestamp) classes
// include nothing yet as want to see if our class includes all it needs
// reltimes brings in string, vector etc.

#include "reltimes.hpp"
#include <iostream> // as test rig needs to output

// minimal test
void dotests(int iteration) {
    timestamp present, past, future;
    
    past.t.tv_sec  = present.t.tv_sec -  60; // into prev minute
    future.t.tv_sec = present.t.tv_sec + 60; // into next minute
    reltimes now; // create an instance
    
    std::vector<std::string> all_labels = now.all_range();   
    std::vector<std::string> cur_labels = now.curr_range(); 
    std::vector<std::string> prev_labels = now.prev_range();
    std::vector<std::string> range_labels;
    std::vector<std::string> ::iterator i;
    


    if(all_labels.size() == 0 || cur_labels.size() == 0 ||  prev_labels.size() == 0) {
        std::cout << iteration << " empty vectors" << std::endl;
        exit(1);
    }
    if(all_labels.size() != (cur_labels.size() + prev_labels.size())) {
        std::cout << iteration << " vector size mismatch" << std::endl;
        exit(1);
    }
    if(past.t.tv_sec > now.start("this_minute")) {
        std::cout << iteration << " past not old enough" << std::endl;
        exit(1);
    }
    if(future.t.tv_sec < now.end("this_minute")) { 
        std::cout << iteration << " future not new enough" << std::endl;
        exit(1);
    }
    // expect to find now in "this_minute"
    range_labels = now.in_range(present);
    if((i = find(range_labels.begin(), range_labels.end(),"this_minute")) ==  range_labels.end()) {
        std::cout << iteration << " present is not in this_minute" << std::endl;
        exit(1);
    }
    range_labels = now.in_range(past);
    if((i = find(range_labels.begin(), range_labels.end(),"this_minute")) !=  range_labels.end()) {
        std::cout << iteration << " past is in this_minute" << std::endl;
        exit(1);
    }
    if((i = find(range_labels.begin(), range_labels.end(),"prev_minute")) ==  range_labels.end()) {
        std::cout << iteration << " past is not in prev_minute " << past.t.tv_sec << " " << now.start("prev_minute") << "-" << now.end("prev_minute") <<  std::endl;
        exit(1);
    }
    range_labels = now.in_range(future);
    if((i = find(range_labels.begin(), range_labels.end(),"this_minute")) !=  range_labels.end()) {
        std::cout << iteration << "future is in this_minute" << std::endl;
        exit(1);
    }
}

int main() {
    for(int i = 0; i < 100000; i++)
        dotests(i);
    // all tests ok
    exit(0);
}

