/***************************************************************************
                          interpolator.cpp  -  description
                             -------------------
    begin                : Sat Mar 30 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
   ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

	Copyright (c) 2013 Charilaos Kalogirou.
	All rights reserved.

	Redistribution and use in source and binary forms are permitted
	provided that the above copyright notice and this paragraph are
	duplicated in all such forms and that any documentation,
	advertising materials, and other materials related to such
	distribution and use acknowledge that the software was developed
	by Charilaos Kalogirou. The name of the
	Charilaos Kalogirou may not be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/

#include "interpolator.h"
#include "tokenizer.h"

#if 0
CInterpolator::CInterpolator(){
}
CInterpolator::~CInterpolator(){
}


void CInterpolatorVector3::insert(T &node, F32 nodeKey){
	vector<CVector3>::iterator nodeIter;
	vector<F32>::iterator keyIter;
	
	for(nodeIter = nodes.begin(), keyIter = keys.begin() ; nodeIter != nodes.end() ; ++nodeIter, ++keyIter){
		if(*keyIter >= nodeKey){
			nodes.insert(nodeIter, 1, node);
			keys.insert(keyIter, 1, nodeKey);
			return;
		}
	}
	nodes.push_back(node);
	keys.push_back(nodeKey);
}

CVector3 CInterpolatorVector3::getAt(F32 key){
	S32 i;

	if(keys.size() == 0)
		return CVector3(0.0, 0.0, 0.0);

	if(key < keys[0])
		return nodes[0];
		
	for(i = 0 ; i < keys.size() - 1 ; i++){
		if(key >= keys[i] && key < keys[i+1]){
			F32 s = keys[i];
			F32 e = keys[i+1];
			
			if(s == e)return nodes[i];
			
			F32 f = (key - s) / (e - s);
			return nodes[i] + (nodes[i+1] - nodes[i]) * f;
		}
	}
	
	return nodes[nodes.size() - 1];
}
#endif
#if 0
void CInterpolatorVector3::parseNodes(const string &buff){
	CTokenizerNoComments t(buff);
	t.addSeparator("(");
	t.addSeparator(")");
	t.addSeparator(",");
	
	nodes.clear();
	keys.clear();

	while(t.nextToken()){
		CVector3 node;
		F32 key = (F32)atof(t.getToken().c_str());
		t.expect("(");
		t.nextToken(); node.x = (F32)atof(t.getToken().c_str()); t.expect(",");
		t.nextToken(); node.y = (F32)atof(t.getToken().c_str()); t.expect(",");
		t.nextToken(); node.z = (F32)atof(t.getToken().c_str());
		t.expect(")");
		
		insert(node, key);
	}
}
#endif
