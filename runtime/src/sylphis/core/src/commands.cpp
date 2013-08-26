
#include "commands.h"


CCommandBase &CCommandSet::registerCommand(const CCommandBase &c){
	return *new CCommandBase();
}

CCommandBase &CCommandSet::findCommand(const string &n){
	return *new CCommandBase();
}
