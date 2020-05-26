// Student Name: Damla Nisa CEVIK
// Student ID: 150170712

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "TestSuite.h"
#include "AllTests.h"
using namespace std;


int minimum(int first, int second, int third) {
    if (first <= second && first <= third) return first;
    else if (second <= first && second <= third) return second;
    else return third;
}


int TestSuite::highest_coverage_index() {
    int highest_covered_test_case_index = -1;
    int max_covered = 0;
    for (unsigned long i = 0; i < integer_frequency_profiles.size(); i++) {
        int counter = 0;
        for (unsigned long j = 0; j < integer_frequency_profiles[i].size(); j++)
            if (integer_frequency_profiles[i][j] != 0) counter++;
        if (counter > max_covered) {
            highest_covered_test_case_index = (int)i;
            max_covered = counter;
        }
    }
    return highest_covered_test_case_index;
}


void TestSuite::levenshtein() {
    int highest_covered_test_case_index = highest_coverage_index();
    for (unsigned long k = 0; k < ordered_sequence.size(); k++) {
        if (k == highest_covered_test_case_index) levenshtein_distances.push_back(-1);
        else {
            int length1 = (int)ordered_sequence[highest_covered_test_case_index].size();
            int length2 = (int)ordered_sequence[k].size();
            int dp_table[length1 + 1][length2 + 1];
            for (int i = 0; i <= length1; i++) {
                for (int j = 0; j <= length2; j++) {
                    if (i == 0)
                        dp_table[i][j] = j;
            
                    else if (j == 0)
                        dp_table[i][j] = i;

                    else if (ordered_sequence[highest_covered_test_case_index][i - 1] == ordered_sequence[k][j - 1])
                        dp_table[i][j] = dp_table[i - 1][j - 1];
            
                    else
                        dp_table[i][j] = 1 + minimum(dp_table[i][j - 1], dp_table[i - 1][j], dp_table[i - 1][j - 1]); // insert, remove, replace
                }
            }
            levenshtein_distances.push_back(dp_table[length2][length1]);
        }
    }
    
    vector<pair<int, int> > pair_vector;
    int first_test_case = -1;
    for (unsigned long i = 0; i < levenshtein_distances.size(); ++i) {
        if (levenshtein_distances[i] == -1) first_test_case = int(i);
        pair_vector.push_back(make_pair(levenshtein_distances[i], i));
    }

    sort(pair_vector.begin(), pair_vector.end());
    ordered_result.push_back(first_test_case + 1);
    
    for (unsigned long i = pair_vector.size() - 1; i > 0; i--)
        if(first_test_case != pair_vector[i].second)
            ordered_result.push_back(pair_vector[i].second + 1);
}


void TestSuite::order_sequences() {
    for (unsigned long i = 0; i < this->integer_frequency_profiles.size(); i++) {
        vector<pair<int, int>> paired_vector;
        for (int j = 0; j < integer_frequency_profiles[i].size(); ++j) paired_vector.push_back(make_pair(integer_frequency_profiles[i][j], j));

        sort(paired_vector.begin(), paired_vector.end());
        vector<int> temp;
        int counter = 0;
        for (int j = 0; j < paired_vector.size(); j++) {
            temp.push_back(paired_vector[j].second + 1);
            counter += paired_vector[j].first;
        }
        ordered_sequence.push_back(temp);
    }
    this->levenshtein();
}

void AllTests::string_to_integer() {
    for (unsigned long i = 0; i < this->selected_test_ids.size(); i++) {
        for (unsigned long j = 0; j < tests[selected_test_ids[i] - 1].frequency_profiles.size(); j++) {
            stringstream line(tests[selected_test_ids[i] - 1].frequency_profiles[j]);
            string token;
            vector<int> temp;
            while (getline(line, token, '-')) temp.push_back(stoi(token));
            tests[selected_test_ids[i] - 1].integer_frequency_profiles.push_back(temp);
            temp.clear();
        }
        tests[selected_test_ids[i] - 1].order_sequences();
    }
}

void AllTests::knapsack() {
    int number_of_tests = int(this->tests.size());
    int table[number_of_tests + 1][this->maximum_running_time + 1];

    for (int i = 0; i <= number_of_tests; i++)
        for (int j = 0; j <= this->maximum_running_time; j++) {
            if (i == 0 || j == 0)
                table[i][j] = 0;
            else if (this->tests[i - 1].running_time <= j)
                table[i][j] = max(this->tests[i - 1].bugs_detected +
                              table[i - 1][j - this->tests[i - 1].running_time], table[i - 1][j]);
            else
                table[i][j] = table[i - 1][j];
        }

    int total_bugs_detected = table[number_of_tests][this->maximum_running_time];
    int time = this->maximum_running_time;
    
    for (int i = number_of_tests; i > 0 && total_bugs_detected > 0; i--) {
        if (total_bugs_detected == table[i - 1][time])
            continue;
        else {
            this->selected_test_ids.push_back(i);
            this->total_running_time += this->tests[i-1].running_time;
            total_bugs_detected = total_bugs_detected - this->tests[i - 1].bugs_detected;
            time = time - this->tests[i - 1].running_time;
        }
    }
}

void AllTests::print_solution() {
    int bugs = 0;
    cout << "Total amount of running time: " << total_running_time << endl;
    for (unsigned long i = 0; i < this->selected_test_ids.size(); i++)
        bugs += this->tests[selected_test_ids[i] - 1].bugs_detected;
    cout << "Total bugs detected: " << bugs << endl << endl;
    for (unsigned long i = this->selected_test_ids.size(); i > 0; --i) {
        cout << "TS" << this->selected_test_ids[i-1] << ": ";
        bugs += this->tests[selected_test_ids[i-1]].bugs_detected;
        for (unsigned long j = 0; j < this->tests[selected_test_ids[i-1] - 1].ordered_result.size(); j++)
            cout << this->tests[selected_test_ids[i-1]-1].ordered_result[j] << " ";
        cout << endl;
    }
}

void read_file(const char* file_name) {
    ifstream test_data_file;
    test_data_file.open(file_name);
    AllTests all_tests;
    if (test_data_file.is_open() && test_data_file.good()) {
        string line;
        string token;
        char delimiter = ' ';
        getline(test_data_file, line);
        stringstream ss(line);
        getline(ss, token, delimiter);
        getline(ss, token, delimiter);
        getline(ss, token, delimiter);
        getline(ss, token, delimiter);
        getline(ss, token, delimiter);
        all_tests.maximum_running_time = stoi(token);
        getline(test_data_file, line);
        getline(test_data_file, line);
        while (getline(test_data_file, line)) {
            TestSuite test_suite;
            stringstream ss(line);
            getline(ss, token, delimiter);
            getline(ss, token, delimiter);
            test_suite.bugs_detected = stoi(token);
            getline(ss, token, delimiter);
            test_suite.running_time = stoi(token);
            while (getline(ss, token, delimiter)) test_suite.frequency_profiles.push_back(token);
            all_tests.tests.push_back(test_suite);
        }
    }
    all_tests.knapsack();
    all_tests.string_to_integer();  // calculate ordered sequences
    all_tests.print_solution();
}


int main(int argc, const char * argv[]) {
    read_file(argv[1]);
    return 0;
}
