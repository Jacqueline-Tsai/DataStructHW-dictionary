#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <set>
using namespace std;
class Hash {
public:
	set<string> data[10000];
	Hash() {
		
	}
	int hash(const string& s) {
		int p = 31;
		int m = 10000;//1e9 + 9
		long long power_of_p = 1;
		long long hash_val = 0;
		for (int i = 0; i < s.length(); i++) {
			hash_val = (hash_val + (s[i] - 'a' + 1) * power_of_p) % m;
			power_of_p = (power_of_p * p) % m;
		}
		return hash_val;
	}
	void insert(string& s) {
		int hash_val = hash(s);
		data[hash_val].insert(s);
	}
	bool in(const string& s) {
		int hash_val = hash(s);
		if (data[hash_val].find(s) != data[hash_val].end())return true;
		else return false;
	}
};
void modifyStr1(Hash& dictionary, set<string>& ans, const string& curStr) {
	if (dictionary.in(curStr))ans.insert(curStr);
	if (curStr.size() == 0) return;
	string letter = "a";
	for (int i = 0;i < 26;i++) {
		// add
		for (int j = 0;j <= curStr.size();j++) {
			string s = curStr;
			s.insert(j, letter);
			if (dictionary.in(s))ans.insert(s);
		}
		//replace
		for (int j = 0;j < curStr.size();j++) {
			string s = curStr;
			if (s[j] == letter[0])continue;
			s[j] = letter[0];
			if (dictionary.in(s))ans.insert(s);
		}
		// erase
		for (int j = 0;j < curStr.size();j++) {
			string s = curStr;
			s.erase(s.begin() + j);
			if (dictionary.in(s))ans.insert(s);
		}
		// reverse
		for (int j = 1;j < curStr.size();j++) {
			string s = curStr;
			swap(s[j - 1], s[j]);
			if (dictionary.in(s))ans.insert(s);
		}
		letter[0] ++;
	}
}
void modifyStr0(Hash& dictionary, set<string>& ans, const string& curStr) {
	set<string> passed;
	string letter = "a";
	for (int i = 0;i < 26;i++) {
		// add
		for (int j = 0;j <= curStr.size();j++) {
			string s = curStr;
			s.insert(j, letter);
			if (passed.find(s) == passed.end()) {
				modifyStr1(dictionary, ans, s);
				passed.insert(s);
			}
		}
		//取代
		for (int j = 0;j < curStr.size();j++) {
			string s = curStr;
			if (s[j] == letter[0])continue;
			s[j] = letter[0];
			if (passed.find(s) == passed.end()) {
				modifyStr1(dictionary, ans, s);
				passed.insert(s);
			}
		}
		// erase
		for (int j = 0;j < curStr.size();j++) {
			string s = curStr;
			s.erase(s.begin() + j);
			if (passed.find(s) == passed.end()) {
				modifyStr1(dictionary, ans, s);
				passed.insert(s);
			}
		}
		// reverse
		for (int j = 1;j < curStr.size();j++) {
			string s = curStr;
			swap(s[j - 1], s[j]);
			if (passed.find(s) == passed.end()) {
				modifyStr1(dictionary, ans, s);
				passed.insert(s);
			}
		}
		letter[0] ++;
	}
}
int main() {
	
	Hash dictionary;
	fstream inDic("dictionary.txt"), inTest("input.txt");
	
	string finStr;
	clock_t start = clock();
	while (getline(inDic, finStr)) {
		if (finStr.substr(0,3) == ";;;")  continue;
		dictionary.insert(finStr);
	}
	inDic.close();

	ofstream ansOut("my_answer.csv");
	ansOut << "word,answer";
	while (getline(inTest, finStr)) {
		set<string> ans;
		ansOut << endl << finStr << ",";
		if (dictionary.in(finStr)) {
			ansOut << "OK";	continue;
		}
		modifyStr0(dictionary, ans, finStr);
		if (ans.size() == 0) {
			ansOut << "NONE";	continue;
		}		
		ansOut << *ans.begin();
		for (set<string>::iterator a = ++ans.begin();a != ans.end();a++)ansOut<< " " << *a ;
	}

	cout << "total time : " << clock() - start << endl;
}