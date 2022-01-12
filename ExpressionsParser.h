#ifndef EXPRESSIONSPARSER_EXPRESSIONSPARSER_H
#define EXPRESSIONSPARSER_EXPRESSIONSPARSER_H

#include <string>
#include <vector>



class ExpressionTree{
public:
    class lexeme{
    public:
        enum operation{
            data, mul, add, sub, div, eof, left_bracket, right_bracket
        };

        operation opr;
        std::string x;

        lexeme() : opr(eof), x("") {}
        lexeme(operation opr) : opr(opr), x("") {}
        lexeme(const std::string &s) : opr(data), x(s) {}


        std::string to_string(){
            if(opr == data){
                return x;
            }
            if(opr == mul){
                return "*";
            }
            if(opr == div){
                return "/";
            }
            if(opr == add){
                return "+";
            }
            if(opr == sub){
                return "-";
            }
        }

    };


    lexeme get_lexeme(std::string &s, int &i){
        while(i < s.size() && s[i] == ' '){
            i += 1;
        }
        if(i >= s.size()){
            i += 1;
            return lexeme(lexeme::eof);
        }
        if(s[i] == '*'){
            i += 1;
            return lexeme(lexeme::mul);
        }
        if(s[i] == '/'){
            i += 1;
            return lexeme(lexeme::div);
        }
        if(s[i] == '+'){
            i += 1;
            return lexeme(lexeme::add);
        }
        if(s[i] == '-'){
            i += 1;
            return lexeme(lexeme::sub);
        }
        if(s[i] == '('){
            i += 1;
            return lexeme(lexeme::left_bracket);
        }
        if(s[i] == ')'){
            i += 1;
            return lexeme(lexeme::right_bracket);
        }
        std::string x;
        for(; i < s.size() && '0' <= s[i] && s[i] <= '9'; ++i){
            x.push_back(s[i]);
        }
        return lexeme(x);
    }



    class node{
    public:
        lexeme lxm;
        int l = -1;
        int r = -1;

        node() {}
        node(const lexeme &lxm) : lxm(lxm) {}
    };

    int root;
    std::vector<node> g;
    lexeme lst_lexeme;


    int from_infix(std::string &s, int &i){
        if(lst_lexeme.opr == lexeme::data){
            node new_node(lst_lexeme);
            g.push_back(new_node);
            lst_lexeme = get_lexeme(s, i);
            return(int)(g.size()) - 1;
        }
        int v = -1;
        auto nd = lexeme::eof;
        if(lst_lexeme.opr == lexeme::left_bracket){
            nd = lexeme::right_bracket;
            lst_lexeme = get_lexeme(s, i);
        }
        v = from_infix(s, i);
        while(lst_lexeme.opr != nd){
            g.push_back(node(lst_lexeme));
            lst_lexeme = get_lexeme(s, i);
            int u = (int)(g.size()) - 1;
            int c = from_infix(s, i);
            g[u].l = v;
            g[u].r = c;
            v = u;
        }
        lst_lexeme = get_lexeme(s, i);
        return v;
    }


    void from_infix(std::string &s){
        int i = 0;
        lst_lexeme = get_lexeme(s, i);
        auto nd = lexeme::eof;
        int v = from_infix(s, i);
        while(lst_lexeme.opr != nd){
            g.push_back(node(lst_lexeme));
            lst_lexeme = get_lexeme(s, i);
            int u = (int)(g.size()) - 1;
            int c = from_infix(s, i);
            g[u].l = v;
            g[u].r = c;
            v = u;
        }
        root = v;
    }


    void to_infix(int v, std::string &res){
        if(g[v].l == -1){
            res += g[v].lxm.to_string();
            return;
        }
        res += "(";
        to_infix(g[v].l, res);
        res += g[v].lxm.to_string();
        to_infix(g[v].r, res);
        res += ")";
    }


    void to_infix(std::string &res){
        res.clear();
        to_infix(root, res);
    }


    void to_postfix(int v, std::string &res){
        if(g[v].l == -1){
            res += g[v].lxm.to_string();
            return;
        }
        to_postfix(g[v].l, res);
        res.push_back(' ');
        to_postfix(g[v].r, res);
        res.push_back(' ');
        res += g[v].lxm.to_string();
    }


    void to_postfix(std::string &res){
        res.clear();
        to_postfix(root, res);
    }


    void to_prefix(int v, std::string &res){
        if(g[v].l == -1){
            res += g[v].lxm.to_string();
            return;
        }
        res += g[v].lxm.to_string();
        res.push_back(' ');
        to_prefix(g[v].l, res);
        res.push_back(' ');
        to_prefix(g[v].r, res);
    }


    void to_prefix(std::string &res){
        res.clear();
        to_prefix(root, res);
    }


    void from_postfix(std::string &s){
        g.clear();
        int i = 0;
        lst_lexeme = get_lexeme(s, i);
        std::vector<int> d;
        while(lst_lexeme.opr != lexeme::eof){
            if(lst_lexeme.opr == lexeme::data){
                g.push_back(node(lst_lexeme));
                d.push_back(g.size() - 1);
                lst_lexeme = get_lexeme(s, i);
                continue;
            }
            int v = g.size();
            g.push_back(node(lst_lexeme));
            g[v].r = d.back();
            d.pop_back();
            g[v].l = d.back();
            d.pop_back();
            d.push_back(v);
            lst_lexeme = get_lexeme(s, i);
        }
        root = d[0];
    }


    int from_prefix(std::string &s, int &i){
        int v = g.size();
        if(lst_lexeme.opr == lexeme::data){
            g.push_back(node(lst_lexeme));
            lst_lexeme = get_lexeme(s, i);
            return v;
        }
        g.push_back(node(lst_lexeme));
        lst_lexeme = get_lexeme(s, i);
        int l = from_prefix(s, i);
        int r = from_prefix(s, i);
        g[v].l = l;
        g[v].r = r;
        return v;
    }


    void from_prefix(std::string &s){
        g.clear();
        int i = 0;
        lst_lexeme = get_lexeme(s, i);
        root = from_prefix(s, i);
    }



};






#endif //EXPRESSIONSPARSER_EXPRESSIONSPARSER_H
