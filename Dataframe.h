#ifndef DATAFRAME_H
#define DATAFRAME_H
#include <string>
#include <fstream>
#include <vector>
using namespace std;

typedef struct Element {
    bool is_numeric = false;
    string object;
    double numeric_value;
}Element;

typedef struct Line {
    vector<Element> elements;
}Line;

typedef struct Column {
    string label;
    vector<Element> elements;
}Column;

class Dataframe {
public:
    int load_from_file(string filepath);
    vector<string> labels;
    vector<Line> dataframe;
    vector<Column> col_dataframe;
    void populate_labels(string line);
    void print_labels();
    void populate_dataframe(string line);
    void print_dataframe();
    void print_line(Line line);
    int get_col_index(string col_name);
    void fix_datatypes();
    void populate_df_cols();
    void print_df_cols();
    vector<string> get_numeric_columns();
    vector<string> get_categorical_columns();
    void print_numeric_columns();
    void print_categorical_columns();
    void delete_line(int index);
    Line correct_line(Line line, string col_remove);
    void remove_column(string col_name);
    void remove_label(string label);
};



#endif //DATAFRAME_H
