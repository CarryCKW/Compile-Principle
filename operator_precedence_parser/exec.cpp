#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <cstdio>
#include <queue>
#include <vector>
#include <stack>
#include <map>
#include <set>
using namespace std;
const int N = 1000;
char *grammarFile = "grammar.in";
string gra[N];
int Len;
map<char, set<char> > firstVt;
map<char, set<char> > lastVt;
set<char> Vn;//非终结符集合 
set<char> Vt;//终结符集合 
struct node {
	char a;
	char b;
	int rel;
};
map<pair<char, char>, node> compareTbl;

/*
(i+i)*i
EE+F
ET
TT*F
TF
Fi
F(E) 
*/
void initG();
void findFirstVt();
void findLastVt();
bool isVt(char c);
void evaCompTbl();
void showCmpTbl();
void check(string s);
void check2(string s);
char findMergeStr(string s);
char findMergeStrByDFS(string s);
string getStkStr(stack<char> stk);
bool dfs(string tgt, char st, string cur, int limDep);

int main() {
	
	initG();
	findFirstVt();
	findLastVt();
	evaCompTbl();
	Vt.insert('#');
	showCmpTbl();
	
	string s0 = "i+i#";
	string s1 = "i+i)*i#";
	string s2 = "(i+i)*i#";
	string s3 = "((i+i)+(i+i))*i#";
	check2(s1);
	
	return 0;
}

void initG() {
	freopen(grammarFile, "r", stdin);
	int n;
	cin>>n;
	Len = n;
	int i=0;
	while(n--) {
		cin>>gra[i++];
		string s = gra[i-1];
		
		for(int j=0;j<s.length();++j) {
			if(s[j]>='A' && s[j]<='Z') {
				Vn.insert(s[j]);
			}
			else{
				Vt.insert(s[j]);
			}
		}
	}
}

void findFirstVt(){
	freopen("first.in","r",stdin);
	int i=0;
	cin>>i;
	string s;
	while(i--){
		cin>>s;
		char key = s[0];
		for(int j=1;j<s.length();++j) {
			firstVt[key].insert(s[j]);
		}
	}
}

void findLastVt(){
	freopen("last.in","r",stdin);
	int i=0;
	cin>>i;
	string s;
	while(i--){
		cin>>s;
		char key = s[0];
		for(int j=1;j<s.length();++j) {
			lastVt[key].insert(s[j]);
		}
	}
}

bool isVt(char c){
	return Vt.count(c)>0?true:false;
}

void evaCompTbl(){
	// < 1
	// = 2
	// > 3
	//5.6.1  a =. b
	for(int i=0;i<Len;++i) {
		string g = gra[i].substr(1);
//		cout<<g<<g.length()<<endl;
		if(g.length()<2) {
			continue;
		}
		for(int j=0;j<g.length();){
			if(isVt(g[j])) {
				if(j+1 <g.length() && !isVt(g[j+1]) && j+2<g.length() && isVt(g[j+2])){
					//A -> ...aRb...
					pair<char, char> p;
					p.first = g[j];
					p.second = g[j+2];
					compareTbl[p] = {g[j], g[j+2], 2};
//					cout<<"i:"<<i<<"j:"<<j<<endl;
					j=j+3;
					continue;
				}
				if(j+1 < g.length() && isVt(g[j+1])) {
					//A -> ...ab...
					pair<char, char> p;
					p.first = g[j];
					p.second = g[j+1];
					compareTbl[p] = {g[j], g[j+1], 2};
					j=j+2;
					continue;
				}
			}
			j++;
		}
	} 
	
	// 5.6.2 a <. b
	for(int i=0;i<Len;++i) {
		string g = gra[i].substr(1);
		if(g.length()<2) {
			continue;
		}
		for(int j=0;j<g.length();) {
			if(isVt(g[j])) {
				// A -> ...aR  , b ∈ FirstVt(R)
				if(j+1 < g.length() && !isVt(g[j+1])) {
					set<char> fvt = firstVt[g[j+1]];
					set<char>::iterator it;
					char a = g[j];
					for ( it=fvt.begin() ; it != fvt.end(); it++ ) {
						char b = *it;
						pair<char, char> p;
						p.first = a;
						p.second = b;
						compareTbl[p] = {a, b, 1};
					}
					j = j+2;
					continue;
				} 
			}
			j++;
		}
	}
	
	//5.6.3 a >. b
	for(int i=0;i<Len;++i) {
		string g = gra[i].substr(1);
		if(g.length()<2) {
			continue;
		}
		for(int j=0;j<g.length();) {
			if(!isVt(g[j])) {
				// A -> ...Rb  , a ∈ LastVt(R)
				if(j+1 < g.length() && isVt(g[j+1])) {
					set<char> lvt = lastVt[g[j]];
					set<char>::iterator it;
					char b = g[j+1];
					for ( it=lvt.begin() ; it != lvt.end(); it++ ) {
						char a = *it;
						pair<char, char> p;
						p.first = a;
						p.second = b;
						compareTbl[p] = {a, b, 3};
					}
					j = j+2;
					continue;
				} 
			}
			j++;
		}
	}
	
	//扩充 # 的关系
	char b = '#'; 
	set<char>::iterator it1;
	for(it1=Vt.begin() ; it1 != Vt.end(); it1++ ) {
		char a = *it1;
		pair<char, char> p;
		p.first = b;
		p.second = a;
		compareTbl[p] = {b, a, 1};  //# < vt
		p.first = a;
		p.second = b;
		compareTbl[p] = {a, b, 3};  //vt > #
	}
	
}

void showCmpTbl() {
	set<char>::iterator it1;
	set<char>::iterator it2;
	set<char> Vt2 = Vt;
	int k=0;
	for(it1=Vt.begin() ; it1 != Vt.end(); it1++ ) {
		char aa = *it1;
		for(it2=Vt2.begin() ; it2 != Vt2.end(); it2++ ) {
			char bb = *it2;
			pair<char, char> p;
			p.first = aa;
			p.second = bb;
			
			if(compareTbl[p].rel == 0) {
				//不存在 
				continue;
			}
			
			cout<<compareTbl[p].a;
			if(compareTbl[p].rel == 1) {
				cout<<" < ";
			}else if(compareTbl[p].rel == 2){
				cout<<" = ";
			}else if(compareTbl[p].rel==3) {
				cout<<" > ";
			}
			cout<<compareTbl[p].b<<endl;
		}
	}
	cout<<"-----------------------"<<endl;
} 

void check(string s) {
	
	stack<char> stk;
	stk.push('#');
	int i=0;
//	stk.push(s[0]);
	int k=15;
	while(k--) {
		char a = s[i++];
		char vv; // 栈顶终结符 （也可能是因为栈顶是非终结符，导致为第二个位置的终结符） 
		if(isVt(stk.top())) {
			vv = stk.top();
		}else{
			if(stk.size()!=1) {
				char tmp = stk.top();
				stk.pop();
				vv = stk.top();
				stk.push(tmp);
			}
			if(stk.size()==1) {
				vv = stk.top();
			}
		}
		if(a=='#') {
			cout<<"OK..."<<endl;
//			break;
		}
		cout<<"vv:"<<vv;
		cout<<" a:"<<a<<endl;
		pair<char, char> p;
		p.first = vv;
		p.second = a;
		int prim =  compareTbl[p].rel;
		if(prim == 1) {//vv < a
			stk.push(a);
		}
		if(prim == 2) {// =
			stk.push(a);
		}
		if(prim == 3) {// > 
			string s = "";
			while(1) {
				char tp = stk.top();
				if(!isVt(tp)&&tp!='#') {
					continue;
				}
				stk.pop();
				pair<char, char> p;
				p.first = tp;
				p.second = vv;
				int prim =  compareTbl[p].rel;
				if(prim == 1) {
					// <
					stk.push(tp);
					string gs = "";
					gs.assign(s.rbegin(), s.rend());
					//寻找规约字串
					char newHead = findMergeStr(gs);
					stk.push(newHead);
					cout<<newHead<<" -> "<<gs<<endl;
					break;
				}
				s+=tp;
			}
		}
		
	}
	
}

void check2(string s) {
	cout<<"analysis string :" <<s<<endl;
	stack<char> stk;
	stk.push('0');
	stk.push('#');
	
	bool flag = true;
	int i=0; //s[i]
	while(flag) {
		char a = s[i++];
		char poschar;
		if(stk.top()=='#' || isVt(stk.top())) {
			poschar = stk.top();
		}
		else{
			char temp = stk.top();
			stk.pop();
			poschar = stk.top();
			stk.push(temp);
		}
		
		if(a =='#') {
			flag = false;
		}
		
//		cout<<"poschar:" <<poschar<<" a: "<<a<<endl;
		
		pair<char, char> p;
		p.first = poschar;
		p.second = a;
		int prim =  compareTbl[p].rel;
		if(prim == 1 || prim == 2) {
			stk.push(a);
		}
		else if(prim == 3) {
			i--;
			//规约
			string s1 = "";
			if(poschar != stk.top()) {
				s1 += stk.top();
				stk.pop();
				s1 += stk.top();//vt
				stk.pop();
			}
			if(poschar == stk.top()) {
				s1 += poschar;
				stk.pop();
			}
			while(1) {
				char pp = stk.top();
				pair<char, char> p;
				p.first = pp;
				p.second = poschar;
				int prim2 =  compareTbl[p].rel;
				if(prim2 == 1 ) {
					break;
				}
				else{
					s1 += pp;
					stk.pop();
//					if(poschar == ')') {
//						cout<<pp<< " ";
//					}
					if(isVt(pp)) {
						poschar = pp;
					} 
				}
			}
			string s2 = "";
			s2.assign(s1.rbegin(), s1.rend());
			char newHead = findMergeStr(s2);
			if(newHead == '~' ) {
				cout<<"find no merge str, process is wrong..."<<endl;
				return ;
			}
			stk.push(newHead);
			cout<<newHead<<" -> "<<s2<<endl;
			
		}
		else {
			//error
			cout<<"error..."<<endl;
//			cout<<"prim: "<<prim<<endl;
			return ;
		}
		string con = getStkStr(stk);
		cout<<"now stk string: "<<con<<endl; 
	}
	
	//最后栈中 # ... #  
	string tmps = "";
	while(stk.top()!='#') {
		tmps += stk.top();
		stk.pop();
	}
	string rs = "";
	rs.assign(tmps.rbegin(), tmps.rend());
	char newHead = findMergeStr(rs);
	stk.push(newHead);
	cout<<newHead<<" -> "<<rs<<endl;
	string con = getStkStr(stk);
	cout<<"now stk string: "<<con<<endl; 
	
	if(flag==false) {
		cout<<"analysis complete, process is right..."<<endl;
	}		
	
}

char findMergeStr(string s){
	for(int i=0;i<Len;++i) {
		if(gra[i].substr(1)==s) {
//			cout<<"meth: s:"<<s<<endl;
			return gra[i][0];
		}
	}
	return '~';
}

char findMergeStrByDFS(string s) {
	
}

bool dfs(string tgt, char st, string cur, int limDep) {
	if(cur==tgt) {
		return true;
	}
	if(limDep > 10) {
		return false;
	}
	
}

string getStkStr(stack<char> stk) {
	string s = "";
	while(stk.size() > 0) {
		s += stk.top();
		stk.pop();
	}
	string s1 = "";
	s1.assign(s.rbegin(), s.rend());
	return s1;
}


















