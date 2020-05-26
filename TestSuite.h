// Student Name: Damla Nisa CEVIK
// Student ID: 150170712

#ifndef TestSuite_h
#define TestSuite_h

#include <vector>
using namespace std;

class TestSuite {
public:
    int bugs_detected;
    int running_time;
    vector<vector<int>> integer_frequency_profiles;
    vector<string> frequency_profiles;
    vector<vector<int>> ordered_sequence;
    vector<int> levenshtein_distances;
    vector<int> ordered_result;
    int highest_coverage_index();
    void order_sequences();
    void levenshtein();
};

#endif /* TestSuite_h */
