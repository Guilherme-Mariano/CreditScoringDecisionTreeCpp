
#ifndef HELPER_H
#define HELPER_H
#include <string>
#include <vector>
using namespace std;

class Helper {
public:
    static bool check_numeric(string object);
    static bool check_string_in_vector(vector<string> vec, string str);
    static vector<double> get_min_max_in_vector(vector<double> vec);
};



#endif //HELPER_H
