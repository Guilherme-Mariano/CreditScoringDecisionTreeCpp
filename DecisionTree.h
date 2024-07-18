#ifndef DECISIONTREE_H
#define DECISIONTREE_H
#include "Dataframe.h"

typedef struct Split {
    bool is_numeric = false;
    string label;
    string categorical;
    double numerical;
    bool greater_than_numerical = false;
    int nr_defaults = -1;
}Split;

class DecisionTree {
public:
    explicit DecisionTree(Dataframe dataframe);
    Dataframe df;
    vector<Split> splits;
    void print_splits(int depth);
    void print_splits();
private:

    void makesplits(int depth);
    Split find_best_split();
    vector<string> get_all_categorical_possibilities(string col_name);
    void print_cat_possibilities(string col_name);
    void print_numeric_minmax(string col_name);
    Split get_best_split_numeric();
    Split get_best_split_categorical();
    int get_default_cat(string col_name, string possibilitie);
    void print_best_split_cat();
    vector<double> get_numbers_in_element_vec(vector<Element> vec);
    int get_number_default_numeric_higher(string col_name, double num);
    int get_number_default_numeric_lower(string col_name, double num);
    void print_best_split_num();
    void make_cat_split(Split sp);
    void make_num_split(Split sp);
};



#endif //DECISIONTREE_H
