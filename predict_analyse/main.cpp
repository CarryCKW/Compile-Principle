/*
 * created by carryckw on 2021.4.26
 */

#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

const int N = 100;
string predict_table[N];
int len;
int T;

void initPreTbl();
void solve(string s);
stack<char> getTbl(char a, char b);
bool isVT(char c);
void error();
bool hasTbl(char a, char b);

int main() {
	initPreTbl();
	
	freopen("a_data.in","r",stdin);
	
	string s;
	while(cin>>s) {
		for(int i=0;i<s.length();++i) {
			if(s[i]>='0' && s[i]<='9') {
				s[i]='i';
			}
			if(s[i]=='-') {
				s[i]='+';
			}
		}
		
		cout<<"s: "<<s<< " ans: "<<endl;
		solve(s);
	}
	
	return 0;
	
}

void initPreTbl() {
	freopen("predict_table.in","r",stdin);
	string s;
	int i = 0;
	cin>>T;
	while(T--) {
		cin>>s;
		
		predict_table[i++] = s;
//		cout<<predict_table[i-1]<<endl;
	}
	len = i;
	fclose(stdin);
	return ;
}

bool hasTbl(char a, char b) {
	if(b>='0' && b<='9') {
		b = 'i';
	}
	for(int i=0;i<len;++i) {
		if(predict_table[i][0]==a && predict_table[i][1]==b) {
			return true;
		}
	}
	return false;
}

stack<char> getTbl(char a, char b) {
	if(b>='0' && b<='9') {
		b = 'i';
	}
	stack<char> res;
	string s = "";
	for(int i=0;i<len;++i) {
		if(predict_table[i][0]==a && predict_table[i][1]==b) {
			s = predict_table[i].substr(2);
			break;
		}
	}
//	cout<<s;
	if(s.length() == 1 && s=="~") {
		return res;
	} 
	for(int i=0;i<s.length();++i) {
		res.push(s[i]);
	}
	return res;
}

void solve(string s) {
	stack<char> stk;
	stk.push('#');
	stk.push('E');
	
	//s[p]
	int p = 0;
	while(p<s.length()) {
		char a = s[p];
		char X = stk.top();
//		cout<<"X: "<<X<<" a:"<<a<<endl;
		stk.pop();
		if(isVT(X)) {
			if(X==a) {
				p++;
				continue;
			}
			else{
				error();
				return ;
			}
		}
		else{
			if(X=='#') {
				if(X==a) {
					cout<<"Ok"<<endl;
					return ;
				}
				else{
					error();
					return ;
				}
			}
			else{
				if(hasTbl(X,a)) {
					stack<char> tbl = getTbl(X,a);
					string s2 = "";
					while(!tbl.empty()) {
						char ch = tbl.top();
						tbl.pop();
						stk.push(ch);
						s2 += ch;
					}
					if(s2.length()==0) {
						s2 = "~";
					}
					string s0 ="";
					s0.assign(s2.rbegin(),s2.rend());
					cout<<X<<" -> "<<s0<<endl;
				}
				else{
					error();
					return ;
				}
				
			}
		}
			
	}
	
	
	
}

bool isVT(char c) {
	if(c=='i' || (c>='0' && c<='9') || c=='*' || c=='+' || c=='-' || c=='(' || c== ')') {
		return true;
	}
	return false;
}
void error() {
	cout<<"error"<<endl;
}
