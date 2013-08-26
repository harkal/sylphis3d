#ifndef _BSPTREE_H_
#define _BSPTREE_H_

#include <list>
#include <vector>
#include "tree.h"

using namespace std;

class CBspNode;
class CPolygon;
class CPlaneManager;

class CBspTree{
public:
	CBspTree();

	CBTree<CBspNode *>::Node *getRoot();

	void setInitialValues(list<CPolygon *> *pol, CPlaneManager *planeManager);

	list<CPolygon *> *getPolygons();
	void reset();
	void buildFirstBsp();
	void buildSecondBsp();
private:
	vector<bool> selectedPlanes;
	bool firstBsp;
	CBTree<CBspNode *> bTree;
	//this is used to keep all the pointers for memory clean up
	list<CPolygon *>  *polygons;

	CPlaneManager *pManager;

	//for memory clean up
	void brakePolygons(CBspNode *nodeToSplit, CBspNode *potitiveNode, CBspNode *negativeNode);
	CPolygon *chooseSplitPolygon(const list<CPolygon *> poly);

	void insertToTree(CBspNode *node, CBTree<CBspNode *>::Node *treeNode);
	void freeTree(CBTree<CBspNode *>::Node *node);
	void getLeafPolygons(list<CPolygon *> *l, CBTree<CBspNode *>::Node *node);
};

#endif


