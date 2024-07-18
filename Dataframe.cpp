#include "Dataframe.h"

#include <iostream>

#include "Helper.h"

int Dataframe::load_from_file(string filepath) {
    ifstream inputFile(filepath);
    if(!inputFile) {
        cerr << "Error opening file";
        return -1;
    }
    int i = 0;
    string line;
    while(getline(inputFile, line)) {
        if(i == 0) {
            populate_labels(line);
            i++;
        }
        populate_dataframe(line);
    }

    inputFile.close();
    return 0;
}
void Dataframe::populate_labels(string line) {
    string str = "";
    for(int i = 0; i <= line.size(); i++) {
        if(line[i] == '\0') {
            labels.insert(labels.end(), str);
            break;
        }
        if(line[i] == ',') {
            if(i == 0) line = "index";
            labels.insert(labels.end(), str);
            str = "";
            continue;
        }
        str += line[i];
    }
}
void Dataframe::print_labels() {
    for(int i = 0; i < labels.size(); i++) {
        cout << labels.at(i) << "\n";
    }
}
void Dataframe::populate_dataframe(string line) {
    string str = "";
    Line lin;
    for(int i = 0; i <= line.length(); i++) {
        if(line[i] == '\0') {
            Element elem;
            elem.object = str;
            lin.elements.insert(lin.elements.end(), elem);
            break;
        }
        if(line[i] == ',') {
            Element elem;
            elem.object = str;
            lin.elements.insert(lin.elements.end(), elem);
            str = "";
            continue;
        }
        str += line[i];
    }
    dataframe.insert(dataframe.end(), lin);
}
void Dataframe::print_dataframe() {
    for(int i = 0; i < dataframe.size(); i++) {
        print_line(dataframe.at(i));
    }
}
void Dataframe::print_line(Line line) {
    for(int i = 0;i < line.elements.size(); i++) {
        cout << "|| " << labels.at(i) << " " << line.elements.at(i).object << " ||";
    }
    cout << "\n";
}
int Dataframe::get_col_index(string col_name) {
    for(int i = 0; i < labels.size(); i++) {
        if(labels.at(i) == col_name) return i;
    }
    return -1;
}
void Dataframe::populate_df_cols() {
    vector<Column> new_df;
    for(int i = 0; i < labels.size(); i++) {
        Column col;
        col.label = labels.at(i);
        int col_index = get_col_index(labels.at(i));
        for(int j = 0; j < dataframe.size(); j++) {
            col.elements.insert(col.elements.end(), dataframe.at(j).elements.at(col_index));
        }
        new_df.insert(new_df.end(), col);
    }
    col_dataframe = new_df;
}
void Dataframe::print_df_cols() {
    for(int i = 0; i < col_dataframe.at(0).elements.size(); i++) {
        for(int j = 0; j < col_dataframe.size(); j++) {
            if(col_dataframe.at(j).elements.at(i).is_numeric) {
                cout << "|| " << col_dataframe.at(j).label << " " << col_dataframe.at(j).elements.at(i).numeric_value << " ||";
                continue;
            }
            cout << "|| " << col_dataframe.at(j).label << " " << col_dataframe.at(j).elements.at(i).object << " ||";
        }
        cout << "\n";
    }
}
void Dataframe::fix_datatypes() {
    for(int i = 0; i < dataframe.size(); i++) {
        for(int j = 0; j < dataframe.at(0).elements.size(); j++) {
            if(Helper::check_numeric(dataframe.at(i).elements.at(j).object)) {
                dataframe.at(i).elements.at(j).is_numeric = true;
                double value;
                if(dataframe.at(i).elements.at(j).object == "") value = 0;
                else value = stod(dataframe.at(i).elements.at(j).object);
                dataframe.at(i).elements.at(j).numeric_value = value;
                dataframe.at(i).elements.at(j).object = "";
            }
        }
    }
}

vector<string> Dataframe::get_numeric_columns() {
    vector<string> numeric_cols;
    bool flag = true;
    for(int i = 0; i < col_dataframe.size(); i++) {
        for(int j = 1; j < col_dataframe.at(0).elements.size(); j++) {
            if(!(col_dataframe.at(i).elements.at(j).is_numeric)) {
                flag = false;
            }
        }
        if(flag) {
            numeric_cols.insert(numeric_cols.end(), col_dataframe.at(i).label);
        }
        flag = true;
    }

    return numeric_cols;
}

vector<string> Dataframe::get_categorical_columns() {
    vector<string> num_cols = get_numeric_columns();
    vector<string> categorical_cols;
    for(int i = 0; i < labels.size(); i++) {
        if(Helper::check_string_in_vector(num_cols, labels.at(i))) {
            continue;
        }
        categorical_cols.insert(categorical_cols.end(), labels.at(i));
    }
    return categorical_cols;
}

void Dataframe::print_numeric_columns() {
    vector<string> num_cols = get_numeric_columns();
    for(int i = 0; i < num_cols.size(); i++) {
        cout << "\n" << num_cols.at(i);
    }
}

void Dataframe::print_categorical_columns() {
    vector<string> categorical_cols = get_categorical_columns();
    for(int i = 0; i < categorical_cols.size(); i++) {
        cout << "\n" << categorical_cols.at(i);
    }
}

void Dataframe::delete_line(int index) {
    dataframe.erase(dataframe.begin() + index);
}

Line Dataframe::correct_line(Line line, string col_remove) {
    Line new_line;
    for(int i = 0; i < line.elements.size(); i++) {
        if(i == get_col_index(col_remove)) continue;
        new_line.elements.insert(new_line.elements.end(), line.elements.at(i));
    }
    return new_line;
}


void Dataframe::remove_column(string col_name) {
    vector<Line> new_df;
    for(int i = 0; i < dataframe.size(); i++) {
        Line nl = correct_line(dataframe.at(i), col_name);
        new_df.insert(new_df.end(), nl);
    }
    remove_label(col_name);
    dataframe = new_df;
}

void Dataframe::remove_label(string label) {
    for(int i = 0; i < labels.size(); i++) {
        if(labels.at(i) == label) {
            labels.erase(labels.begin() + i);
        }
    }
}

