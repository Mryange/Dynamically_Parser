#include <bits/stdc++.h>
#include<conio.h>
#include"tokenize.h"
#include"var.h"
#include"parse.h"
#include"parsejson.h"
using namespace std;

void run() {
    ifstream fin("test.ycc");
    stringstream ss;
    ss << fin.rdbuf();
    //cout << ss.str() << endl;
    string s = ss.str(); s += "                 ";
    auto vec = tokenize::tokenize(s);
    parse::run(vec);
}
int main() {
    run();
    cout << endl;
    cout << "请输入任意键结束....";
    _getch();
    return 0;
}
