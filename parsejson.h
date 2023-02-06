#pragma once
#include<bits/stdc++.h>
#include"var.h"
#include"tokenize.h"
using namespace std;


namespace parse_json {
    vector<token>tokens;
    int id = 0;
    token get() {
        return tokens[id];
    }
    void next() {
        id++;
    }
    void eat(string s) {
        if (get().value != s) {
            cout << "error : cant find " << s << " in line:" << get().line << endl;
            exit(0);
        }
        next();
    }
    var parse_obj();
    var parse_arr();
    var parse_con();
    var parse_map();
    var parse_obj() {
        var obj;
        if (get().value == "[") {
            eat("[");
            obj = parse_arr();
            eat("]");
            return obj;
        }
        if (get().value == "{") {
            eat("{");
            obj = parse_map();
            eat("}");
            return obj;
        }
        obj = parse_con(); next();
        return obj;
    }

    var parse_con() {
        string s = get().value;
        if (count(s.begin(), s.end(), '"')) {
            return { s.substr(1, s.length() - 2) };
        }
        if (count(s.begin(), s.end(), '.') || count(s.begin(), s.end(), 'f') || count(s.begin(), s.end(), 'e')) {
            stringstream ss; ss << s;
            double x; ss >> x;
            return { x };
        }
        stringstream ss; ss << s;
        int x;
        ss >> x;
        return { x };
    }
    var parse_arr() {
        var arr; arr.type = ARR;
        while (get().value != "]") {
            arr.add(parse_obj());
            if (get().value == "]")break;
            eat(",");
        }
        return arr;
    }
    var parse_map() {
        var map; map.type = MAP;
        while (get().value != "}") {
            var key = parse_obj();
            eat(":");
            var val = parse_obj();
            map.put(key, val);
            if (get().value == "}")break;
            eat(",");
        }
        return map;
    }
    

    var parse_json(vector<token>_tokens) {
        id = 0;
        tokens = _tokens;
        return parse_obj();
    }
}










