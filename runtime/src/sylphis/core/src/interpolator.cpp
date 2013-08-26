/***************************************************************************
                          interpolator.cpp  -  description
                             -------------------
    begin                : Sat Mar 30 2002
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
