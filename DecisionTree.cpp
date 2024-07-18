#include "DecisionTree.h"

#include <iostream>

#include "Helper.h"

DecisionTree::DecisionTree(Dataframe dataframe) {
    df = dataframe;
}

vector<string> DecisionTree::get_all_categorical_possibilities(string col_name) {
    int col_index = df.get_col_index(col_name);
    vector<string> possibilities;
    for(int i = 1; i < df.col_dataframe.at(col_index).elements.size();i++) {
        if(!Helper::check_string_in_vector(possibilities, df.col_dataframe.at(col_index).elements.at(i).object)) {
            possibilities.insert(possibilities.end(), df.col_dataframe.at(col_index).elements.at(i).object);
        }
    }
    return possibilities;
}

void DecisionTree::print_cat_possibilities(string col_name) {
    vector<string> pos = get_all_categorical_possibilities(col_name);
    for(int i = 0; i < pos.size(); i++) {
        cout << "\n" << pos.at(i);
    }
}

void DecisionTree::print_numeric_minmax(string col_name) {
    int col_index = df.get_col_index(col_name);
    vector<double> vec;
    for(int i = 0; i < df.col_dataframe.at(col_index).elements.size(); i++) {
        vec.insert(vec.end(), df.col_dataframe.at(col_index).elements.at(i).numeric_value);
    }
    vector<double> ans = Helper::get_min_max_in_vector(vec);
    cout << "\n Numeric minmax";
    for(int i = 0; i < ans.size(); i++) {
        cout << "\n" << ans.at(i);
    }
}

int DecisionTree::get_default_cat(string col_name,string possibilitie) {
    int col_index = df.get_col_index(col_name);
    int target_index = df.get_col_index("status");
    int n_default = 0;
    for(int i = 0; i < df.col_dataframe.at(col_index).elements.size(); i++) {
        if(df.col_dataframe.at(col_index).elements.at(i).object == possibilitie) {
            if(df.col_dataframe.at(target_index).elements.at(i).object == "default") {
                n_default++;
            }
        }
    }
    return n_default;
}

Split DecisionTree::find_best_split() {
    Split best;
    for(int i = 0; i < df.col_dataframe.size(); i++) {
        Split cat = get_best_split_categorical();
        Split num = get_best_split_numeric();
        if(cat.nr_defaults > best.nr_defaults || num.nr_defaults > best.nr_defaults) {
            if(cat.nr_defaults > num.nr_defaults) {
                best = cat;
            }else {
                best = num;
            }
        }
    }
    return best;
}

Split DecisionTree::get_best_split_categorical() {
    vector<string> cat_cols = df.get_categorical_columns();
    string best_cat_col = "";
    string best_cat_pos = "";
    int best_default_value = -1;
    for(int i = 0; i < cat_cols.size(); i++) {
        if(cat_cols.at(i) == "status") continue;
        vector<string> cat_pos = get_all_categorical_possibilities(cat_cols.at(i));
        for(int j = 0; j < cat_pos.size(); j++) {
            int nr_def = get_default_cat(cat_cols.at(i), cat_pos.at(j));
            if(nr_def > best_default_value) {
                best_default_value = nr_def;
                best_cat_col = cat_cols.at(i);
                best_cat_pos = cat_pos.at(j);
            }
        }
    }
    Split cat_split;
    cat_split.label = best_cat_col;
    cat_split.categorical = best_cat_pos;
    cat_split.nr_defaults = best_default_value;
    return cat_split;
}

Split DecisionTree::get_best_split_numeric() {
    vector<string> num_cols = df.get_numeric_columns();
    string best_n_col = "";
    double best_n_value = -1;
    int best_default_value = -1;
    bool greater_than;
    for(int i = 1; i < num_cols.size(); i++) {
        int col_index = df.get_col_index(num_cols.at(i));
        string col_name = num_cols.at(i);
        vector<double> n_rep = get_numbers_in_element_vec(df.col_dataframe.at(col_index).elements);
        vector<double> minmax = Helper::get_min_max_in_vector(n_rep);
        int min = minmax.at(0);
        int max = minmax.at(1);
        if(i > 5000) break;
        for(int j = min; j < max; j++) {

            double num = j;
            int l = get_number_default_numeric_lower(num_cols.at(i), num);
            int h = get_number_default_numeric_higher(num_cols.at(i), num);
            if(h > best_default_value || l > best_default_value) {
                if(h > l) {
                    greater_than = true;
                    if(best_default_value == -1) best_default_value = 0;
                    best_default_value = h;
                    best_n_value = num;
                    best_n_col = num_cols.at(i);
                }
                if(l > h) {
                    greater_than = false;
                    if(best_default_value == -1) best_default_value = 0;
                    best_default_value = l;
                    best_n_value = num;
                    best_n_col = num_cols.at(i);
                }
            }
        }
    }
    Split sp;
    sp.is_numeric = true;
    sp.greater_than_numerical = greater_than;
    sp.label = best_n_col;
    sp.nr_defaults = best_default_value;
    sp.numerical = best_n_value;

    return sp;

}

void DecisionTree::print_best_split_num() {
    Split sp = get_best_split_numeric();
    cout << "\n Label: " << sp.label << " Numeric Value: " << sp.numerical << " Nr Defaults: " << sp.nr_defaults;
    if(sp.greater_than_numerical) {
        cout << " Higher ";
    }else {
        cout << " Lower";
    }
}

int DecisionTree::get_number_default_numeric_lower(string col_name, double num) {
    int col_index = df.get_col_index(col_name);
    int target_index = df.get_col_index("status");
    int n_def = -1;
    for(int i = 0; i < df.col_dataframe.at(col_index).elements.size(); i++) {
        if(df.col_dataframe.at(col_index).elements.at(i).numeric_value < num) {
            if(df.col_dataframe.at(target_index).elements.at(i).object == "default") {
                if(n_def == -1) n_def = 0;
                n_def++;
            }
        }
    }
    return n_def;
}

int DecisionTree::get_number_default_numeric_higher(string col_name, double num) {
    int col_index = df.get_col_index(col_name);
    int target_index = df.get_col_index("status");
    int n_def = -1;
    for(int i = 0; i < df.col_dataframe.at(col_index).elements.size(); i++) {
        if(df.col_dataframe.at(col_index).elements.at(i).numeric_value > num) {
            if(df.col_dataframe.at(target_index).elements.at(i).object == "default") {
                if(n_def == -1) n_def = 0;
                n_def++;
            }
        }
    }
    return n_def;
}


vector<double> DecisionTree::get_numbers_in_element_vec(vector<Element> vec) {
    vector<double> ans;
    for(int i = 0; i < vec.size(); i++) {
        ans.insert(ans.end(), vec.at(i).numeric_value);
    }
    return ans;
}


void DecisionTree::print_best_split_cat() {
    Split sp = get_best_split_categorical();
    cout << "\n Label: " << sp.label << " Categorie: " << sp.categorical << " Nr Defaults: " << sp.nr_defaults;
    if(sp.greater_than_numerical) {
        cout << " Tem que ser maior ";
    }else {
        cout << " Tem que ser menor ";
    }
}


void DecisionTree::makesplits(int depth) {
    if(depth > df.col_dataframe.size()) depth = df.col_dataframe.size();
    for(int i = 0; i < depth; i++) {
        Split best = find_best_split();
        if(best.is_numeric) {
            make_num_split(best);
        }else {
            make_cat_split(best);
        }
        splits.insert(splits.end(), best);
    }
}

void DecisionTree::make_cat_split(Split sp) {
    string col = sp.label;
    string possibility = sp.categorical;
    int col_index = df.get_col_index(col);
    vector<Line> new_df;
    for(int i = 0; i < df.dataframe.size(); i++) {
        if(df.dataframe.at(i).elements.at(col_index).object != possibility) {
            continue;
        }
        new_df.insert(new_df.end(), df.dataframe.at(i));
    }
    df.dataframe = new_df;
    df.remove_column(col);
    df.populate_df_cols();
}

void DecisionTree::make_num_split(Split sp) {
    string col = sp.label;
    double value = sp.numerical;
    int col_index = df.get_col_index(col);
    vector<Line> new_df;
    for(int i = 0; i < df.dataframe.size(); i++) {
        if(sp.greater_than_numerical) {
            if(df.dataframe.at(i).elements.at(col_index).numeric_value < value) {
                continue;
            }
            new_df.insert(new_df.end(), df.dataframe.at(i));
        }else {
            if(df.dataframe.at(i).elements.at(col_index).numeric_value > value) {
                continue;
            }
            new_df.insert(new_df.end(), df.dataframe.at(i));
        }
    }
    df.dataframe = new_df;
    df.remove_column(col);
    df.populate_df_cols();
}

void DecisionTree::print_splits(int depth) {
    makesplits(depth);
    for(int i = 0; i < splits.size(); i++) {
        if(splits.at(i).is_numeric) {
            if(splits.at(i).greater_than_numerical) {
                cout << " \nNumeric Split Col: " << splits.at(i).label << " Greater than: " << splits.at(i).numerical << " Nr Defaults" << splits.at(i).nr_defaults << "\n";
            }else {
                cout << " \nNumeric Split Col: " << splits.at(i).label << " Less than: " << splits.at(i).numerical << " Nr Defaults" << splits.at(i).nr_defaults << "\n";
            }
        }else {
            cout << " \nCategorical Split Col: " << splits.at(i).label << " Possibilitie: " << splits.at(i).categorical << " Nr Defaults" << splits.at(i).nr_defaults << "\n";
        }
    }
}

void DecisionTree::print_splits() {
    int depth = df.col_dataframe.size();
    makesplits(depth);
    for(int i = 0; i < splits.size(); i++) {
        if(splits.at(i).is_numeric) {
            if(splits.at(i).greater_than_numerical) {
                cout << " \nNumeric Split Col: " << splits.at(i).label << " Greater than: " << splits.at(i).numerical << " Nr Defaults" << splits.at(i).nr_defaults << "\n";
            }else {
                cout << " \nNumeric Split Col: " << splits.at(i).label << " Less than: " << splits.at(i).numerical << " Nr Defaults" << splits.at(i).nr_defaults << "\n";
            }
        }else {
            cout << " \nCategorical Split Col: " << splits.at(i).label << " Possibilitie: " << splits.at(i).categorical << " Nr Defaults" << splits.at(i).nr_defaults << "\n";
        }
    }
}

