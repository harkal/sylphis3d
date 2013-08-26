
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <string>
#include <list>
using namespace std;

class CCommandBase {
public:
	CCommandBase(){};
	CCommandBase(string &n){
		setName(n);
	};

	void setName(string &n){ name = n; };
	string &getName(){ return name; };
private:
	string name;
};

class CCommandSet : public list<CCommandBase> {
public:
	CCommandBase &registerCommand(const CCommandBase &c);
	CCommandBase &findCommand(const string &n);
private:
};

#endif

