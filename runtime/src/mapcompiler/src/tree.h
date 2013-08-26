/***************************************************************************
                          tree.h  -  description
                             -------------------
    begin                : Sun Jun 16 2002
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
 ***************************************************************************/


#ifndef _TREE_H
#define _TREE_H

using namespace std;

#ifdef MEMPROF
#include "mmgr.h"
#endif

template < class T > 
class CBTree {
public:
	class Node {
	public:
		T data;
		Node *kids[2];
		Node(){
			kids[0] = 0;
			kids[1] = 0;
		};
		Node *child(const int kidNo){
			return kids[kidNo];
		};
		Node **childRef(const int kidNo){
			return &kids[kidNo];
		};
		///
		//  free all the subTree from that node
		///
		void freeIt(){
			if (kids[0]) {
				kids[0]->freeIt();
				delete kids[0];
			}
			if (kids[1]) {
				kids[1]->freeIt();
				delete kids[1];
			}
		};

		bool isLeaf(){
			if (kids[0])
				return false;
			if (kids[1])
				return false;
			return true;
		};
		///
		//  call this to insert at kid-kidId data
		///
		void insertTo(const int kidId, T & insertedData){
			if (kids[kidId]) {
				kids[kidId]->freeIt();
				delete kids[kidId];
			}
			kids[kidId] = new Node();
			kids[kidId]->data = insertedData;
		};

		//call this for not by reference
		void insertToNotByRF(const int kidId, T insertedData){
			if (kids[kidId]) {
				kids[kidId]->freeIt();
				delete kids[kidId];
			}
			kids[kidId] = new Node();
			kids[kidId]->data = insertedData;
		};
		void print(){
			for (int i = 0; i < 2; i++) {
				if (!kids[i])
					continue;

				cout << "=";

				kids[i]->print();
			}
			cout << (float) data << endl;
		};
		~Node() {
		};
	};

	Node * rootNode;
	CBTree() {
		rootNode = 0;
	};                          /*default binary tree */
	///
	//  returns true if tree empty and since that isEmpty will not return true
	//  return false if tree is not empty and  does nothing else further.---reset() should be called to clear the tree
	///
	bool init(){
		if (isEmpty()) {
			rootNode = new Node();
			return true;
		}
		return false;
	};

	bool isEmpty(){
		if (rootNode)
			return false;
		return true;
	};

	///
	//  makes tree empty
	///
	void reset(){
		if (!rootNode)
			return;
		rootNode->freeIt();
		delete rootNode;
		rootNode = 0;
	};

	~CBTree() {
		reset();
	};
};

#endif
