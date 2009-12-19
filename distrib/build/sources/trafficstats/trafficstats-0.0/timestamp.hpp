#ifndef TIMESTAMP_INC
#define TIMESTAMP_INC
#include <sys/time.h>
#include <string>
#include <map>
// so we can make vectors and maps of timestamps, cant use just a naked timeval
// for these uses
// just a wrapper round a timeval - which is exposed as timestamp.t
// access the data directly.

class timestamp {
public:
    struct timeval t;
    // no params assume you weant current time
    timestamp() {
        gettimeofday(&t, NULL);
    };
    // from a struct timeval
    timestamp(const struct timeval &x) {
        // structure assign
        t = x;
    };
    // minimal ops for map use
    bool operator < (const timestamp &s) {
        return(t.tv_sec < s.t.tv_sec || t.tv_usec < s.t.tv_usec);
    };
    timestamp & operator = (const timestamp &s) {
        // structure assign
        t = s.t;
        return *this;
    };
    // and from a non class one too
    timestamp & operator = (const struct timeval &s) {
        // structure assign
        t = s;
        return *this;
    };
    // is onme timestamp the same as another
    bool operator == (const timestamp &s) {
        return (t.tv_sec == s.t.tv_sec && t.tv_usec == s.t.tv_usec);
    };
    
};
    
#endif
