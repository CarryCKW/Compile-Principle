/*
1.空弧版本 
*/
#include <string>
#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;
char* file = "states.in";
const int N = 1010;
string cfn[N]; //状态映射集合  同states.in 
vector<char> inputs; // 输入集合 

vector<string> newStates;

string init;
int Len=0;
char E = '~';//空弧 

queue<char> findInitState();
queue<char> findNextState(char cur, char func);
queue<char> nextStatesOneIpt(queue<char> curS, char func);
string getIdentifyCode(queue<char> curS);
void solve();
bool isNewStateExist(string s);
queue<char> findNextStateWithE(char cur, char func); // 包含空弧 

int main() {
	
	solve();
	
	cout<<"新的所有状态为: "<<endl; 
	cout<<init<<endl;
	for(int i=0;i<newStates.size();++i) {
		cout<<newStates[i]<<endl;
	}
	
	return 0;
}

queue<char> findInitState() {
	freopen(file,"r",stdin);
	queue<char> q;
	
	init="";
	cin>>init;
	
	int i=0;
	//输入状态映射关系 
	while(cin>>cfn[i++]){
		char ipt = cfn[i-1][1];
		bool flag = false;
		for(int i=0;i<inputs.size();++i) {
			if(ipt==inputs[i]) {
				 flag = true;
				 break;
			}
		}
		if(!flag && ipt!=E) {
			inputs.push_back(ipt);
//			cout<<"ipt: "<<ipt<<endl;
		}
	} 
	Len = i-1;
	
	for(int i=0;i<init.length();++i) {
		q.push(init[i]);
	}
	
	// 从q 经过任意条空弧到达的状态 q' 
	queue<char> I;
	string s0 = "";
	while(!q.empty()) {
		char c = q.front();
		q.pop();
		if(s0.find(c)==-1) { //去重 
			I.push(c);
			s0 += c;
//			cout<<"i': " <<c<<endl;
		}
		queue<char> ns = findNextState(c, E);
		while(!ns.empty()) {
			char c2 = ns.front();
			ns.pop();
			q.push(c2);
		}
	}
	
	return I;
}


queue<char> findNextState(char cur, char func)  {
	queue<char> n;
	for(int i=0;i<Len;++i) {
		if(cfn[i][0]==cur && cfn[i][1]==func) {
			n.push(cfn[i][2]);
		}
	}
	return n;
}

queue<char> nextStatesOneIpt(queue<char> curS, char func) {
	queue<char> res;
	string s0 = "";
	while(!curS.empty()){
		char c = curS.front();
		curS.pop();
		queue<char> ns = findNextStateWithE(c, func);
		while(!ns.empty()) {
			char c2 = ns.front();
			ns.pop();
			if(s0.find(c2)==-1) {
				s0 += c2;
				res.push(c2);
//				cout<<"c2: "<<c2<<endl; 
			}
		}
	}
	
	return res;
}

string getIdentifyCode(queue<char> curS) {
	int all[N];
	int i=0;
	while(!curS.empty()) {
		char c = curS.front();
		curS.pop();
		all[i++] = c-'0';
//		cout<<all[i-1]<<" ";
	}
	sort(all, all + i);
	string s0 = "";
	for(int j=0;j<i;++j) {
		char c1 = '0' + all[j];
		s0 += c1; 
	} 
//	cout<<"s0: "<<s0;
	
	return s0;
}

void solve() {
	queue<char> I = findInitState();
	queue<queue<char> > allStates;
	allStates.push(I);
	int sizIpt = inputs.size();
	while(!allStates.empty()) {
		queue<char> q = allStates.front();
		allStates.pop();
		string id0 = getIdentifyCode(q);
		for(int i=0;i<sizIpt;++i) {
			queue<char> ns = nextStatesOneIpt(q, inputs[i]);
			if(ns.empty()) {
				continue;
			}
			string id = getIdentifyCode(ns);
			if(!isNewStateExist(id)) {
				newStates.push_back(id);
				allStates.push(ns);
				
			}
			cout<<"state:	" << id0 << "	->	" << id<< "	by	" << inputs[i] << endl;
		}
		
	}
	
}

bool isNewStateExist(string s) {
	for(int i=0;i<newStates.size();++i) {
		if(s==newStates[i]) {
			return true;
		}
	} 
	return false;
}
queue<char> findNextStateWithE(char cur, char func){
	queue<char> q = findNextState(cur, func);
	queue<char> I;
	string s0 = "";
	while(!q.empty()) {
		char c = q.front();
		q.pop();
		if(s0.find(c)==-1) { //去重 
			I.push(c);
			s0 += c;
//			cout<<"i': " <<c<<endl;
		}
		queue<char> ns = findNextState(c, E);
		while(!ns.empty()) {
			char c2 = ns.front();
			ns.pop();
			q.push(c2);
		}
	}
	return I;
}


