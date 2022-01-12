#include "ExpressionsParser.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>


using namespace std;


int main() {
    while(1) {
        string tp;
        getline(cin, tp);
        string exp;
        getline(cin, exp);
        ExpressionTree t;
        if (tp == "infix") {
            t.from_infix(exp);
            string exp1;
            t.to_postfix(exp1);
            cout << exp1 << "\n";
            t.to_prefix(exp1);
            cout << exp1 << "\n";
        } else if(tp == "postfix") {
            t.from_postfix(exp);
            string exp1;
            t.to_infix(exp1);
            cout << exp1 << "\n";
            t.to_prefix(exp1);
            cout << exp1 << "\n";
        } else if(tp == "prefix"){
            t.from_prefix(exp);
            string exp1;
            t.to_infix(exp1);
            cout << exp1 << "\n";
            t.to_postfix(exp1);
            cout << exp1 << "\n";
        }
    }
    return 0;
}

/*
prefix
+ * 2 4 + 1 5
 */