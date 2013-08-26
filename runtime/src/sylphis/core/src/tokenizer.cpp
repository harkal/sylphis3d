/***************************************************************************
                          tokenizer.cpp  -  description
                             -------------------
    begin                : Thu Mar 14 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
   ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

    "Sylphis3D Game Engine" is free software; you can redistribute it and/or 
    modify it under the terms of the GNU General Public License as 
    published by the Free Software Foundation; either version 2 of the 
    License, or (at your option) any later version, 
    plus a special exception:
      Linking "Sylphis3D Game Engine" statically or dynamically with other 
    modules is making a combined work based on this library. Thus, the 
    terms and conditions of the GNU General Public License cover the whole 
    combination. As a special exception, the copyright holders of this 
    library give you permission to link this library with independent 
    modules to produce an executable, regardless of the license terms of 
    these independent modules, and to copy and distribute the resulting 
    executable under terms of your choice, provided that you also meet, 
    for each linked independent module, the terms and conditions of the 
    license of that module. An independent module is a module which is 
    not derived from or based on this program. If you modify this program,
    you may extend this exception to your version of the library, but you 
    are not obligated to do so. If you do not wish to do so, delete this 
    exception statement from your version.

    "Sylphis3D Game Engine" is distributed in the hope that it will be 
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Sylphis3D Game Engine"; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 ***************************************************************************/

#include "tokenizer.h"

CTokenizerBase::CTokenizerBase(const string &b){
	lowerCaseTokens = false;
	line = 1;
	pos = 0;
	token = "";
	endOfStream = false;
	buffer = b + "   ";
}

string CTokenizerBase::lowerCase(const string &a){
	string s = a;
	for(U32 i = 0; i < s.length() ; i++){
		s[i] = s[i] <= 'Z' && s[i] >= 'A' ? s[i] + ('a'-'A') : s[i];
	}
	return s;
}

void CTokenizerBase::nextChar() {
	pos++;
	if(pos >= buffer.length()){
		endOfStream = true;
		pos--;
		return;
	}
	switch (buffer[pos]) {
	case 0:
		endOfStream = true;
		break;
	case '\n':
		line++;
		break;
	case '\r':
		buffer[pos] = ' ';
		break;
	}
}

void CTokenizerBase::expect(const string &token, bool nextAfter){
    if(!nextAfter)
	    nextToken();
	if(getToken() != token)
		throw CTokenizerException(*this, "Expected '" + token + "'");
    if(nextAfter)
	    nextToken();
}

void CTokenizerBase::skipToNextLine() {
	while ( currentChar() != '\n' ){
		nextChar();
		if(endOfStream)return;
	}
	nextChar();
}

void CTokenizerBase::setLowerCaseTokens(bool b){
	lowerCaseTokens = b;
}

void CTokenizerBase::skipToToken(const string &token){
	while ( nextToken() ){
		if(getToken() == token)return;
	}
}

void CTokenizerBase::backToStartOfLine(){
	S32 i = buffer.rfind("\n", pos);
	if(i == string::npos)
		pos = 0;
	else
		pos = i + 1;
}

void CTokenizerBase::bookmark(){
    bookmarkPos = pos;
    bookmarkLine = line;
}

void CTokenizerBase::gotoBookmark(){
    pos = bookmarkPos;
    line = bookmarkLine;
}

void CTokenizerBase::skipWhitespace() {
	while ( currentChar() <= ' ') {
		nextChar();
		if(endOfStream)return;
	}
}

bool CTokenizerBase::nextToken() {
	string newToken;

    newToken = "";

	skipWhitespace();
	if(endOfStream)return false;
	
	// Handle quoted strings
	if(currentChar() == '\"'){
		while(1){
			nextChar();
			if(endOfStream){
				//Exception to handle open quotation
				token = newToken;
				return true;
			}
			if(currentChar() == '\"'){
				nextChar();
				token = newToken;
				return true;
			}
			if(currentChar() == '\n')
				newToken += " ";
			  else
				newToken += currentChar();
		}
	}

	do {
		newToken += currentChar();
		nextChar();
		if(endOfStream)break;
	} while (currentChar() > 32);
	
	token.assign(newToken);
	return true;
}

string CTokenizerBase::getToken() {
    if(endOfStream)
        return "";
	if(lowerCaseTokens)
		token = lowerCase(token);

	return token;
}

S32 CTokenizerBase::getLine(){
	return line;
}

//
// CTokenizer
//

bool CTokenizer::nextToken() {
	S32 lastpos = pos;

	if(!CTokenizerBase::nextToken())return false;

	vector<CSeparator>::iterator iter;
	S32 min = token.length();
	for(iter = separators.begin() ; iter != separators.end() ; iter++){
		S32 i = token.find(iter->sep);
		if(i == string::npos)continue;
		else if(i == 0){
			pos -= token.length() - iter->sep.length();
			token = iter->sep;
			return true;
		} else if(i < min){
			min = i;
		}
	}
	pos -= token.length() - min;
	token.assign(token, 0, min);
	
	return true;
}

void CTokenizer::addSeparator(const CSeparator &s){
	separators.push_back(s);
}

void CTokenizer::addSeparator(const string &s){
	separators.push_back(CSeparator(s));
}

//
// CTokenizerNoComments
//

bool CTokenizerNoComments::nextToken() {
	while(CTokenizer::nextToken()){
		if(token == "//"){
			skipToNextLine();
		} else {
			return true;
		}
	}
	return false;
}

