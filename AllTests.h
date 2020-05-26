// Student Name: Damla Nisa CEVIK
// Student ID: 150170712

#ifndef AllTests_h
#define AllTests_h

#include <vector>
using namespace std;

class AllTests {
public:
    vector<TestSuite> tests;
    int maximum_running_time;
    int total_running_time = 0;
    vector<int> selected_test_ids;
    void knapsack();
    void string_to_integer();
    void print_solution();
};

#endif /* AllTests_h */
