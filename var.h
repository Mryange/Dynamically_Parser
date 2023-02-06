#pragma once
#include<bits/stdc++.h>
using namespace std;
#define INT 0
#define DOU 1
#define STR 2
#define MAP 3
#define ARR 4
class var
{
public:
    int INT_VAL = 0;
    double DOU_VAL;
    char CHA_VAL;
    string STR_VAL;
    vector<var>ARR_VAL;
    struct Cmp
    {
        bool operator () (var const& a, var const& b)const
        {
            if (a.type != b.type)return a.type < b.type;
            else {
                if (a.type == INT)return a.INT_VAL < b.INT_VAL;
                if (a.type == DOU)return a.DOU_VAL < b.DOU_VAL;
                if (a.type == STR)return a.STR_VAL < b.STR_VAL;
                if (a.type == ARR)return a.ARR_VAL < b.ARR_VAL;
                else return a.MAP_VAL < b.MAP_VAL;
            }
        }
    };

    map<var, var, Cmp>MAP_VAL;
    int type = MAP;
    void del() { INT_VAL = DOU_VAL = 0; STR_VAL.clear(), MAP_VAL.clear(); }
    var() {}
    var(int x) { INT_VAL = x; type = INT; }
    var(double x) { DOU_VAL = x; type = DOU; }
    var(string x) { STR_VAL = x; type = STR; }
    var(const char* ch) {
        int n = strlen(ch);
        string s;
        for (int i = 0; i < n; i++)s.push_back(ch[i]);
        STR_VAL = s; type = STR;
    }
    var& operator [] (var id);
    void add(var x) {
        type = ARR;
        ARR_VAL.push_back(x);
    }
    void put(var key, var val) {
        type = MAP;
        MAP_VAL[key] = val;
    }
    int size() {
        if (type == STR)return STR_VAL.length();
        return MAP_VAL.size();
    }
};

var tmp;
var& var::operator[](var id) {
    if (type == STR) {
        string res; res.push_back(STR_VAL[id.INT_VAL]);
        tmp = res;
        return tmp;
    }
    else if (type == ARR) {
        return ARR_VAL[id.INT_VAL];
    }
    type = MAP;
    return MAP_VAL[id];
}
ostream& operator << (ostream& out, const var& v) {
    if (v.type == INT)out << v.INT_VAL;
    if (v.type == DOU)out << v.DOU_VAL;
    if (v.type == STR)out << "" << v.STR_VAL<<"";
    if (v.type == MAP) {
        out << "{";
        for (auto it = v.MAP_VAL.begin(); it != v.MAP_VAL.end(); it++) {
            out << (it->first) << ":" << (it->second);
            if (it != prev(v.MAP_VAL.end()))out << ",";
        }
        out << "}";
    }
    if (v.type == ARR) {
        out << "[";
        for (auto it = v.ARR_VAL.begin(); it != v.ARR_VAL.end(); it++) {
            out << *it;
            if (it != prev(v.ARR_VAL.end()))out << ",";
        }
        out << "]";
    }
    return out;
}
var operator + (var a, var b);
var operator - (var a, var b);
var operator * (var a, var b);
var operator / (var a, var b);

var operator + (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL + b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL + b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL + b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL + b.DOU_VAL;
    }
    if (a.type == STR && b.type == STR)return a.STR_VAL + b.STR_VAL;
    if (a.type == STR && b.type == INT)return a.STR_VAL + to_string(b.INT_VAL);
    if (a.type == STR && b.type == DOU)return a.STR_VAL + to_string(b.DOU_VAL);
    if (a.type == INT && b.type == STR)return to_string(a.INT_VAL) + b.STR_VAL;
    if (a.type == DOU && b.type == STR)return to_string(a.DOU_VAL) + b.STR_VAL;
}

var operator - (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL - b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL - b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL - b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL - b.DOU_VAL;
    }
}

var operator * (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL * b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL * b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL * b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL * b.DOU_VAL;
    }
    if (a.type == STR && b.type == INT) {
        string s = "";
        int n = b.INT_VAL;
        while (n--)s += a.STR_VAL;
        return s;
    }
}

var operator / (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL / b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL / b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL / b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL / b.DOU_VAL;
    }
}


var operator ^ (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return pow(a.DOU_VAL, b.INT_VAL);
        if (b.type == DOU)return pow(a.DOU_VAL, b.DOU_VAL);
    }
    if (a.type == INT) {
        if (b.type == INT)return pow(a.INT_VAL, b.INT_VAL);
        if (b.type == DOU)return pow(a.INT_VAL, b.DOU_VAL);
    }
}

var operator % (var a, var b) {
    return a.INT_VAL % b.INT_VAL;
}

bool operator == (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL == b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL == b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL == b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL == b.DOU_VAL;
    }
    if (a.type == STR && b.type == STR) {
        return a.STR_VAL == b.STR_VAL;
    }
    return 0;
}

bool operator != (var a, var b) {
     if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL != b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL != b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL != b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL != b.DOU_VAL;
    }
    if (a.type == STR && b.type != STR) {
         return (a.STR_VAL != b.STR_VAL);
    }
}

bool operator  && (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL && b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL && b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL && b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL && b.DOU_VAL;
    }
}

bool operator  || (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL || b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL || b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return (a.INT_VAL != 0) || (b.INT_VAL != 0);
        if (b.type == DOU)return (a.INT_VAL != 0) || (b.DOU_VAL != 0);
    }
}

bool operator  < (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL < b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL < b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL < b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL < b.DOU_VAL;
    }
}


bool operator  > (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL > b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL > b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL > b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL > b.DOU_VAL;
    }
}

bool operator  <= (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL <= b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL <= b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL <= b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL <= b.DOU_VAL;
    }
}


bool operator  >= (var a, var b) {
    if (a.type == DOU) {
        if (b.type == INT)return a.DOU_VAL >= b.INT_VAL;
        if (b.type == DOU)return a.DOU_VAL >= b.DOU_VAL;
    }
    if (a.type == INT) {
        if (b.type == INT)return a.INT_VAL >= b.INT_VAL;
        if (b.type == DOU)return a.INT_VAL >= b.DOU_VAL;
    }
}

