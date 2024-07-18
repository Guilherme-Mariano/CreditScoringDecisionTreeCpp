
#include "Helper.h"

bool Helper::check_numeric(string object) {
    for(int i = 0; i < object.length(); i++) {
        if(!(isdigit(object[i])) && object[i] != '.') return false;
    }
    return true;
}

bool Helper::check_string_in_vector(vector<string> vec, string str) {
    if(vec.empty()) return false;
    for(int i = 0; i < vec.size(); i++) {
        if(vec.at(i) == str) return true;
    }
    return false;
}

vector<double> Helper::get_min_max_in_vector(vector<double> vec) {
    double min = 1000;
    double max = -100000;

    for(int i = 0; i < vec.size(); i++) {
        // Trash value
        if(vec.at(i) < 10) continue;
        if(vec.at(i) > max) {
            max = vec.at(i);
        }
        if(vec.at(i) < min) {
            min = vec.at(i);
        }
    }
    vector<double> ans;
    ans.insert(ans.end(), min);
    ans.insert(ans.end(), max);
    return ans;
}

