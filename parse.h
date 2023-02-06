#pragma once
#include<bits/stdc++.h>
#include"tokenize.h"
#include"var.h"
using namespace std;
namespace parse {
	map<string, int> level;
	vector<token>tokens;
	int tokens_id = 0;
	token get() {
		return tokens[tokens_id];
	}
	void next() {
		tokens_id++;
	}
	void jump(int tar) {
		tokens_id = tar;
	}
	int cur() {
		return tokens_id;
	}
	void eat(string need) {
		if (get().value != need) {
			cout << "need token :" << need << "   in  " << tokens_id << endl;
			exit(0);
		}
		next();
	}
	bool has_next() {
		return tokens_id < tokens.size();
	}
	vector<var>parameter;
	map<string, var>global;
	stack<map<string, var>>stk_var;
	var& get_var(string id) {
		if (global.count(id)) {
			return global[id];
		}
		if (!stk_var.top().count(id)) stk_var.top()[id] = 0;
		return stk_var.top()[id];
	}
	void find_func_by_name(string name) {
		tokens_id = 0;
		while (1) {
			if (tokens[tokens_id].value == "def" && tokens[tokens_id + 1].value == name) {
				tokens_id++;
				return;
			}
			tokens_id++;
		}
	}
	stack<int>call_func;

	void ret() {

		jump(call_func.top());
		call_func.pop();

	}

	var parse_func();
	var parse_stm();
	var parse_exp();



	var parse_if();

	var parse_while();
	stack<var>last_if;
	int if_jmp[10005];

	var parse_func() {
		stk_var.push({});
		next();//func name
		eat("(");

		for (var pa : parameter) {
			get_var(get().value) = pa;
			next();
		}
		eat(")");
		eat("{");
		var x = parse_stm();
		eat("}");
		stk_var.pop();
		ret();
		return x;
	}
	var call(string name) {
		call_func.push(cur());
		find_func_by_name(name);
		return parse_func();
	}


	var parse_stm() {
		last_if.push(0);
		while (get().value != "}") {
			if (get().value == "if") {
				int jp = cur();
				eat("if");
				last_if.top() = parse_exp();
				if (last_if.top() != 0) {
					eat("{");
					parse_if();

					eat("}");
				}
				else {
					jump(if_jmp[jp]);
					eat("}");
				}
			}
			else if (get().value == "else") {
				int jp = cur();
				eat("else");
				if (!(last_if.top() != 0)) {
					eat("{");
					parse_if();
					eat("}");
				}
				else {
					jump(if_jmp[jp]);
					eat("}");
				}
			}
			else if (get().value == "while") {
				int jp = cur();
				eat("while");
				var flag = parse_exp();
				if (flag != 0) {
					eat("{");
					parse_while();
				}
				else {
					jump(if_jmp[jp]);
					eat("}");
				}
			}
			else if (get().value == "for") {
				int jp = cur(); eat("for");
				string id = get().value; next();
                if (get().value == ",") {
                    string key = id;
                    eat(",");
                    string val = get().value; next();
                    eat("in");
                    string _map = get().value; next();
                    int head = cur();
                    for (auto it = get_var(_map).MAP_VAL.begin(); it != get_var(_map).MAP_VAL.end(); it++) {
                        get_var(key) = it->first, get_var(val) = it->second;
                        eat("{");
                        parse_stm();
                        eat("}");
                        jump(head);
                    }
                    jump(if_jmp[jp]);
                    eat("}");
                }
                else {
                    eat("in");
                    eat("[");
                    var st = parse_exp(); eat(",");
                    var ed = parse_exp(); eat(",");
                    var d = parse_exp();
                    eat("]");
                    get_var(id) = st;
                    int head = cur();
                    while (get_var(id) <= ed) {
                        eat("{");
                        parse_stm();
                        eat("}");
                        get_var(id) = get_var(id) + d;
                        jump(head);
                    }
                    jump(if_jmp[jp]);
                    eat("}");
                }
				
			}
			else if (get().value == "print") {
				next();
				cout << parse_exp();
			}
			else if (get().value == "println") {
				next();
				if (get().value == ";") {
					cout << endl;
					next();
				}
				else cout << parse_exp() << endl;
			}
			else if (get().value == "global") {
				eat("global");
				global[get().value] = 0;
				next();
				eat(";");
			}
			else if (get().value == "return") {
				eat("return");
				last_if.pop();
				return parse_exp();
			}
			else {
				parse_exp();
			}

		}
		last_if.pop();
		return 0;
	}

	var parse_if() {
		return parse_stm();
	}

	var parse_while() {
		var x = parse_stm();
		jump(if_jmp[cur()]);
		return x;
	}

	var get_con(string s) {
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
	struct mapvar {
		string name;
		var key;
	};
	struct AST_tree {
		var x;
		token tk;
		AST_tree* L, * R;
		bool ismap = 0;
		mapvar mv;
		var& Lvar() {
			if (!ismap)return get_var(tk.value);
			return get_var(mv.name)[mv.key];
		}
		var eval() {
			if (tk.type == OP) {
				if (tk.value == "=") {
					L->Lvar() = R->eval();
					return 0;
				}
				else if (tk.value == "+") {
					return L->eval() + R->eval();
				}
				else if (tk.value == "*") {
					return L->eval() * R->eval();
				}
				else if (tk.value == "==") {
					return L->eval() == R->eval();
				}
				else if (tk.value == "<=") {
					return L->eval() <= R->eval();
				}
				else if (tk.value == "-") {
					return L->eval() - R->eval();
				}
				else if (tk.value == ">") {
					return L->eval() > R->eval();
				}
				else if (tk.value == "!=") {
					var x = L->eval() == R->eval();
					if (x == 0)x = 1;
					else x = 0;
					return x;
				}
                else if (tk.value == "||") {

                    return L->eval() || R->eval();
                }
                else if (tk.value == "/") {
                    return L->eval() / R->eval();
                }
				else if (tk.value == "%") {
					return L->eval() % R->eval();
				}
				else if (tk.value == "&&") {
					return L->eval() && R->eval();
				}
            
				else {
					cout << "cant find op : " << tk.value << endl;
					exit(0);
				}
			}
			else {
				if (!ismap)return x;
				return get_var(mv.name)[mv.key];
			}
		}
	};

	var parse_exp() {
		vector<AST_tree>AST(100);
		stack<token>op_stk;
		stack<int>num_stk;
		int AST_idx = 1;
		int cot = 0;
		while (get().type != KEY) {
			auto t = get();
			if (t.value == ")" && cot == 0)break;
			if (t.value == "]")break;
			next();
			if (t.type == CON) {
				num_stk.push(AST_idx);
				AST[AST_idx].x = get_con(t.value);
				AST[AST_idx].tk = t;
				AST_idx++;
            }
            else if (t.value == "len") {
                eat("(");
                num_stk.push(AST_idx);
                string key = get().value; next();
                AST[AST_idx].x = get_var(key).size();
                t.type = CON;
                AST[AST_idx].tk = t;
                AST_idx++;
                eat(")");                
            }
			else if (t.type == VAR) {
				num_stk.push(AST_idx);
				if (get().value == "[") {
					eat("[");
					var key = parse_exp();
					AST[AST_idx].x = get_var(t.value)[key];
					AST[AST_idx].ismap = true;
					AST[AST_idx].mv = { t.value,key };
					AST[AST_idx].tk = t;
					AST_idx++;
					eat("]");
				}
				else {
					AST[AST_idx].x = get_var(t.value);
					AST[AST_idx].tk = t;
					AST_idx++;
				}
			}
			else if (t.type == FUNC) {
				eat("(");
				if (get().value == ")") {
					eat(")");
					num_stk.push(AST_idx);
					AST[AST_idx].x = call(t.value);
					AST[AST_idx].tk = t;
					AST_idx++;
				}
				else {

					parameter.clear();
					while (get().value != ",") {

						parameter.push_back(parse_exp());
						if (get().value == ")")break;
						eat(",");
					}
					eat(")");
					num_stk.push(AST_idx);
					AST[AST_idx].x = call(t.value);
					AST[AST_idx].tk = t;
					AST_idx++;
					parameter.clear();
				}

			}
			else {
				string ch = t.value;
				if (ch == "(") {
					cot++;
					op_stk.push(t);
				}
				else if (ch == ")") {
					cot--;
					while (op_stk.size() && op_stk.top().value != "(") {
						auto op = op_stk.top(); op_stk.pop();
						int R = num_stk.top(); num_stk.pop();
						int L = num_stk.top(); num_stk.pop();
						num_stk.push(AST_idx);
						AST[AST_idx].tk = op;
						AST[AST_idx].L = &AST[L];
						AST[AST_idx].R = &AST[R];
						AST_idx++;
					}
					op_stk.pop();
				}
				else {
					while (op_stk.size() && level[op_stk.top().value] >= level[t.value]) {
						auto op = op_stk.top(); op_stk.pop();
						int R = num_stk.top(); num_stk.pop();
						int L = num_stk.top(); num_stk.pop();
						num_stk.push(AST_idx);
						AST[AST_idx].tk = op;
						AST[AST_idx].L = &AST[L];
						AST[AST_idx].R = &AST[R];
						AST_idx++;
					}
					op_stk.push(t);
				}
			}
		}
		while (op_stk.size()) {
			auto op = op_stk.top(); op_stk.pop();
			int R = num_stk.top(); num_stk.pop();
			int L = num_stk.top(); num_stk.pop();
			num_stk.push(AST_idx);
			AST[AST_idx].tk = op;
			AST[AST_idx].L = &AST[L];
			AST[AST_idx].R = &AST[R];
			AST_idx++;
		}
		if (get().value == ";")next();
		if (num_stk.size() == 0)return 0;
		return AST[num_stk.top()].eval();
	}


	var run(vector<token>_tokens) {
		tokens = _tokens;
		tokens_id = 0;
		level["!"] = 5;
		level["*"] = level["/"] = 2;
		level["+"] = level["-"] = 1;
		level["="] = -5;
		level["||"] = -3, level["&&"] = -2;
		level["++"] = 100;
		level["--"] = 100;
		level["("] = level[")"] = -100;

		stack<pair<string, int>>stk;
		for (int i = 0; i < tokens.size(); i++) {
			if (tokens[i].value == "if") {
				stk.push({ "if", i });
			}
			if (tokens[i].value == "while") {
				stk.push({ "while", i });
			}
			if (tokens[i].value == "else") {
				stk.push({ "else", i });
			}
			if (tokens[i].value == "for") {
				stk.push({ "for", i });
			}
			if (tokens[i].value == "}") {
				if (stk.size()) {
					if_jmp[stk.top().second] = i;
					if_jmp[i] = stk.top().second;
					stk.pop();
				}
			}
		}
		parameter.clear();
		return call("main");
	}

}

