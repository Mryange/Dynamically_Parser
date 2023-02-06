#pragma once
#include<bits/stdc++.h>
using namespace std;
enum token_type {
    KEY,
    VAR,
    FUNC,
    CON,
    OP,
};
struct token{
    token_type type;
    string value;
    int line;
};
//用于输出
string token_print_map[30] = { "key","var","func","con","op" };

namespace  tokenize {

    string text;
    int pc;
    int cur_line;
    vector<token>tokens;
    bool find(string res) {
        for (int i = 0; i < res.length(); i++) {
            if (res[i] != text[pc + i])return false;
        }
        return true;
    }
    vector<string>op_words = {
       "&&","||",
        "++","--",
         "<=",">=",
         "==","!=",
        "+", "-","*","/",
        ">","<",
         "=",
        "%","!",
        "(",")",
        "len",
        "[","]"
    };
    vector<string> key_words = {
        "def",
        "if",
        "else",
        "while",
        "for",
        "in",
        "println",
        "print",
        "scanf",
        "global",
        "{","}",
        ";",
        ",",   
        ":",
        "return"
    };
    string get_var() {
        string res;
        while (text[pc] == '_' || isalpha(text[pc]) || (text[pc] >= '0' && text[pc] <= '9')) {
            res.push_back(text[pc]);
            pc++;
        }
        return res;
    }   
    string get_con() {
        string res;
        if (text[pc] == '"') {
            res.push_back(text[pc++]);
            while (text[pc] != '"')res.push_back(text[pc++]);
            res.push_back(text[pc++]);
        }
        else {
            set<char> float_need = { 'e','f','.' };//浮点数需要的字符
            while ((text[pc] >= '0' && text[pc] <= '9') || float_need.count(text[pc]))res.push_back(text[pc++]);
        }
        return res;
    }
    set<string>func_set;
    void parse() {
        for (auto key_word : key_words) {
            if (find(key_word)) {
                tokens.push_back({ KEY,key_word ,cur_line});
                pc += key_word.length();
                return;
            }
        }
        for (auto op_word : op_words) {
            if (find(op_word)) {
                tokens.push_back({ OP,op_word,cur_line });
                pc += op_word.length();
                return;
            }
        }
        if ((text[pc] >= '0' && text[pc] <= '9') || text[pc] == '"') {
            tokens.push_back({ CON,get_con(),cur_line });
            return;
        }
        if (tokens.back().value == "def") {
            string v = get_var();
            func_set.insert(v);
            tokens.push_back({ FUNC,v,cur_line });
            return;
        }
        string v = get_var();
        if (func_set.count(v)) {         
            tokens.push_back({ FUNC,v,cur_line });
            return;
        } 
        tokens.push_back({ VAR,v,cur_line });
    }

    vector<token>  tokenize(string s) {
        tokens.clear();
        pc = 0;
        text = s;
        cur_line = 1;
        while (pc < text.length()) {
            while (text[pc] == ' ' || text[pc] == '\n' || text[pc] == '\t') {
                if (text[pc] == '\n') cur_line++; 
                pc++;
            }
            parse();
            while (text[pc] == ' ' || text[pc] == '\n' || text[pc] == '\t') {
                if (text[pc] == '\n') cur_line++;
                pc++;
            }
        }
        return tokens;
    }
}




