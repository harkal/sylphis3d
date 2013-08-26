/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
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

#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <fstream>

#include "bspwriter.h"
#include "bspcreator.h"
#include "loader.h"
#include "entity.h"
#include "portal.h"
#include "bspnode.h"
#include "bsptree.h"

#ifdef MEMPROF
#include "mmgr.h"
#endif


//#define USE_SNAP
//#define USE_ROUND
#define	SNAP_FLOAT_TO_INT    64
#define	SNAP_INT_TO_FLOAT	(1.0f/SNAP_FLOAT_TO_INT)



CBspWriter::CBspWriter( CBspCreator *theBsp, const bool write,const string &fileP, bool neighbors )
{
	bsp=theBsp;
	loader = bsp->getMapLoader();
	pManager = loader->getPlaneManager();
	tj = &bsp->getTJ();
    mFindNeighbors = neighbors;

    list<CEntity *> *lights = loader->getListWithEntities("classname", "light");
    if(lights){
        for(list<CEntity *>::iterator i = lights->begin() ; i != lights->end() ; ++i){
            string sorigin = (*i)->getProperty("origin");
            string sradius = (*i)->getProperty("radius") + " 0 0";
            CVector3 origin, radius;
            sscanf(sorigin.c_str(), "%f %f %f", &origin.x, &origin.y, &origin.z);
            origin.swapHand();
            sscanf(sradius.c_str(), "%f %f %f", &radius.x, &radius.y, &radius.z);
            if(radius.y == 0.0){
                radius.y = radius.z = radius.x;
            }
            radius.swapHand();
            radius.z = -radius.z;
            radius = radius + CVector3(16,16,16);
            CAABoundBox bound;
            bound.setBounds(origin - radius, origin + radius);
            mLightBounds.push_back(bound);
        }
        
        delete lights;
    }

	filePath=loader->getFilePath();
	size_t place=filePath.rfind(".map");
	if(place !=-1)
		filePath=filePath.replace(place, 5, ".cmap", 5);
	else
		filePath+=".cmap";

	if(write)
		writeIt();
}

void CBspWriter::initWrite()
{
	tBSPHeader *header;
	int cpot = sizeof(tBSPHeader) + kMaxLumps*sizeof(tBSPLump);            

	memData.reserve(cpot);
	memData.resize(cpot);

	header = (tBSPHeader *)&memData[0];
	char strid[4]={'C','M','A', 'P'};
	for(int i = 0 ; i < 4 ; i++)
		header->strID[i]=strid[i];
	header->version=17;

	tBSPLump *nl;
	nl = (tBSPLump*)&memData[sizeof(tBSPHeader)];
	for(int i=0;i<kMaxLumps; i++){
		nl->offset=0;
		nl->length=0;
		nl++;
	}

	//this is to get reed of the useless planes
	pManager->initForWritting();
}


void CBspWriter::writeIt()
{

	cout << "writing cmap at "<<filePath<<endl;
	initWrite();    
	cout << "|-------writting data-------|"<<endl;
	writeShaders();

	writeBrushesAndMuchMore();

	writePlanes();
	//writeAll();

    writeNeighbors();

	//THE ENTITIES MUST REMAIN THE LAST LUMP WRITTEN IN ORDER TO NOBSP TO WORK
	writeEntities();
	writeMemData();
}

void CBspWriter::writeNeighbors(){
    return;
}

void CBspWriter::writeNodes(){

	vector<tBSPNode>::iterator tNIter, tNEnd;

	CBTree<CBspNode *>::Node *rootNode = bsp->getBspTree()->getRoot();

	int rootIndex = -addNode(rootNode);
	//--rootIndex;
	tBSPNode *tNodesTW = new tBSPNode[tNodes.size()+1];

	tNIter = tNodes.begin();
	tNEnd = tNodes.end();
	int tnI = 1;
	while(tNIter != tNEnd){
		tNodesTW[tnI] = *tNIter;
		//      cout << "tNodesTW["<<tnI<<"].plane="<<tNodesTW[tnI].plane<<endl;
		for(int i=0;i<3;i++){
			//         cout << "tNodesTW["<<tnI<<"].mins["<<i<<"]="<<tNodesTW[tnI].mins[i]<<endl;
			//         cout << "tNodesTW["<<tnI<<"].maxs["<<i<<"]="<<tNodesTW[tnI].maxs[i]<<endl;
		}
		++tnI;
		++tNIter;
	}
	tNodesTW[0] = tNodesTW[rootIndex];

	writeLump(
		kNodes,
		tNodesTW,
		tNodes.size()+1
		);

	cout <<"| "<< (tNodes.size()+1) << " nodes"   <<endl;
	delete [] tNodesTW;    
}

int CBspWriter::addNode(CBTree<CBspNode *>::Node *node)
{
	if(node->isLeaf()){
		CBspLeaf *leaf = (CBspLeaf *)node->data;
		if(leaf->getIndex() != -1)
			return leaf->getIndex()+1;
		else
			return 0;
	}

	CBspNode *nodedata = node->data;
	tBSPNode newnode;
	newnode.plane = pManager->getWrittingPlaneId(nodedata->getSplitPlane()->getPlaneId());

	CAABoundBox nodeBounds = nodedata->getBounds();
	for(int i=0;i<3;i++){
		newnode.mins[i] = nodeBounds.getMin()[i];
		newnode.maxs[i] = nodeBounds.getMax()[i];    
	}  

	int frontI, backI;
	frontI = addNode(node->child(0));
	backI = addNode(node->child(1));

	//  if(frontI!=0&&backI!=0){
	newnode.back = backI;
	newnode.front = frontI;  
	tNodes.push_back(newnode);
	return -static_cast<int>(tNodes.size());
	//}  
	//return 0;
}

#include "timer.h"

//#define DEBUG_NEIGH
void findUniqueVertices(const vector<CVector3> &original, vector<int> &reorder);
class PolyEdge {
public:
    PolyEdge() : poly(-1), edge(-1) {}
    int poly;
    int edge;
};

template <class T>
class CSparseVector {
    class CNode {
    public:
        unsigned int mIndex;
        T mData;
    };
public:
    CSparseVector(const T &def){
        mDefaultData = def;
    }

    const T &operator[](unsigned int pos) const {
        return T();
    }

    T &operator[](unsigned int pos){
        for(TDataContainer::iterator i = mData.begin() ; i != mData.end() ; ++i){
            if((*i).mIndex == pos)
                return (*i).mData;
        }

        CNode n;
        n.mIndex = pos;
        n.mData = mDefaultData;
        mData.push_back(n);
        return mData.back().mData;
    }
protected:
    typedef list<CNode> TDataContainer;
    TDataContainer mData;
    T mDefaultData;
};

void CBspWriter::writeBrushesAndMuchMore(){
	vector<tBSPBrush> brushes;
	vector<int> brushSides;
	vector<tBSPVertex> vertices;
    vector<int> neighbors;
	vector<tBSPLeaf> leafs;
	vector<tBSPLeaf>::iterator leafsIter, leafsEnd;
	vector<tBSPFace> faces;
	vector<tBSPElement> elements;
	vector<tBSPFace> portalFaces;
	vector<tBSPVertex> portalVertices;
	vector<tBSPArea> tareas;

	vector<int> brushesIndexingForLeafs;
	vector<int> brushesReIndexing;

	tBSPArea newArea;
	vector<tBSPAreaPortal> portals;

	tBSPAreaPortal newportal;
	vector<int> areaPortalIndirection;

	vector<CArea *>::iterator aIter, aEnd;
	list<CBspLeaf *>::iterator lIter, lEnd;  
	vector<int>::iterator sbIter, sbEnd;
	tBSPLeaf newLeaf;
	list<CPolygon *>::iterator polIter, polEnd;  
	vector<CAreaPortal *>::iterator apIter, apEnd;

	int brushId;

	CEntity *mainEntity = loader->getWorldEntity();

	CPortalCreator *pcreator = &bsp->getPortalCreator();

	brushesReIndexing.resize( mainEntity->getBrushesNo(), -1 );

	//here found the structural
	aIter = pcreator->aBegin();
	aEnd = pcreator->aEnd();
	int areaNo=0;
	while(aIter!=aEnd){
		newArea.startLeaf = leafs.size();
		newArea.startElement = elements.size();
		newArea.startFace = faces.size();
		newArea.startPortal = areaPortalIndirection.size();

		lIter = (*aIter)->lBegin();
		lEnd = (*aIter)->lEnd();

		while(lIter!=lEnd){
			int leafsSize = leafs.size();
			CBspLeaf *leaf = *lIter;
			leaf->setIndex(leafsSize);

			newLeaf.leafBrush = brushesIndexingForLeafs.size();

			newLeaf.leafface = faces.size();
			newLeaf.element = elements.size();

			//STRUCTURAL BRUSHES
			sbIter = (*lIter)->sbBegin();
			sbEnd = (*lIter)->sbEnd();
			while(sbIter!=sbEnd){
				brushId = *sbIter;
				if(brushId == -1)
					assert(false);
				CBrush &b = mainEntity->getBrush(brushId);
				if(!b.isStructural())
					addBrushPolygons(&b, &faces, &elements, &vertices, TEXTURE_COOR, USE_MERGE, NO_USE_TJ);

				if(brushesReIndexing[brushId]!=-1){
					brushesIndexingForLeafs.push_back(brushesReIndexing[brushId]);
					++sbIter;
					continue;          
				}

				if(mainEntity->getBrush(brushId).isOpaque()){
					brushesReIndexing[brushId] = brushes.size();
					brushesIndexingForLeafs.push_back(brushesReIndexing[brushId]);
					writeBrush(b, &brushes, &brushSides);
				}

				++sbIter;
			}


			//NOW ADD LEAF'S POLYGONS
			if(areaNo!=0){
				polIter = (*lIter)->pBegin();
				polEnd = (*lIter)->pEnd();
				CPolygon *polygon;
				while(polIter!=polEnd){
					polygon = *polIter;
					if(polygon->isKeep())
						addPolygon(polygon, &faces, &elements, &vertices, WRITE_ELEMENTS, TEXTURE_COOR, NO_USE_TJ);
					++polIter;
				}
			}
			//add the leaf bound box
			//if(areaNo!=0)
			//  cout << "newLeaf.index="<<leaf->getIndex()<<endl;
			for(int j=0;j<3;j++){
				newLeaf.mins[j]=(*lIter)->getBounds().getMin()[j];
				newLeaf.maxs[j]=(*lIter)->getBounds().getMax()[j];
			}

			// end of leaf atribute
			newLeaf.numOfLeafBrushes = brushesIndexingForLeafs.size()-newLeaf.leafBrush;
			newLeaf.numOfLeafFaces = faces.size() - newLeaf.leafface;
			newLeaf.numOfElements = elements.size() - newLeaf.element;
			newLeaf.area = (*lIter)->getArea();

			leafs.push_back(newLeaf);
			++lIter;
		}

		newArea.leafsSize = leafs.size()-newArea.startLeaf;
		newArea.elementsSize = elements.size()-newArea.startElement;
		newArea.facesSize = faces.size()-newArea.startFace;

		//HERE DOWN THE AREAPORTALS
		if(areaNo!=0){    
			apIter = (*aIter)->apBegin();
			apEnd = (*aIter)->apEnd();
			while(apIter!=apEnd){
				CAreaPortal *ap = *apIter;
				int areaPIndex = ap->getIndex();
				if(areaPIndex != -1){
					areaPortalIndirection.push_back(areaPIndex);
					++apIter;
					continue;
				}
				newportal.faceNo = portalFaces.size();
				newportal.areas[0] = (*ap->getArea(FRONT_AREA)->lBegin())->getArea();
				newportal.areas[1] = (*ap->getArea(BACK_AREA)->lBegin())->getArea();

				ap->boundPolygon();
				for(int i=0;i<3;i++){
					newportal.maxs[i] = ap->getPolBounds().getMax()[i];
					newportal.mins[i] = ap->getPolBounds().getMin()[i];
				}
				ap->setIndex(portals.size());
				areaPortalIndirection.push_back(ap->getIndex());      
				portals.push_back(newportal);      
				addPolygon(ap, &portalFaces, 0, &portalVertices, NO_ELEMENTS, NO_TEXTURE_COOR, NO_USE_TJ); //writeElements=false
				++apIter;
			}
		}

		newArea.portalsSize = areaPortalIndirection.size()-newArea.startPortal;
		tareas.push_back(newArea);

		areaNo++;
		++aIter;
	}


	///////NEW CODE FOR ENTITIES PORTES  
	list<CEntity *>::iterator entityIter, entityEnd;
	entityIter = loader->entityBegin();
	entityEnd  = loader->entityEnd();
	vector<tBSPBrushEntity> writeEntities;
	vector<CBrush>::iterator brushIter, brushEnd;
	tBSPBrushEntity bEntity;
	while(entityIter!=entityEnd){
		if(!(*entityIter)->isModelEntity()){
			entityIter++;
			continue;      
		}
		bEntity.startFace = faces.size();
		bEntity.startElement = elements.size();
		bEntity.startBrush = brushes.size();

		processModelEntity(*entityIter);
		brushIter = (*entityIter)->bBegin();
		brushEnd = (*entityIter)->bEnd();

		while(brushIter!=brushEnd){
			//calculate the new tranlated planes
			addBrushPolygons(&(*brushIter), &faces, &elements, &vertices, NO_TEXTURE_COOR, NO_USE_MERGE, NO_USE_TJ);
			writeBrush(*brushIter, &brushes, &brushSides);
			++brushIter;
		}

		bEntity.brushesSize = brushes.size() - bEntity.startBrush;
		bEntity.facesSize = faces.size() - bEntity.startFace;
		bEntity.elementsSize = elements.size() - bEntity.startElement;


		writeEntities.push_back(bEntity);

		entityIter++;
	}

	tBSPBrushEntity *wEntities = new tBSPBrushEntity[writeEntities.size()];

	int eEntitiesNum = writeEntities.size();
	for(int i=0;i<eEntitiesNum;i++){
		wEntities[i] = writeEntities[i];
	}

	///WRITTINE THE BRUSH ENTITIES
	writeLump(
		kBrushEntities,
		wEntities,
		writeEntities.size()
		);
	delete [] wEntities;


	///////NEW CODE FOR ENTITIES

	//padd the cause the areaportals faces will be appended to the faces
	int wPortalNum = portals.size();
	for(int i=0;i<wPortalNum;i++){
		portals[i].faceNo += faces.size();
	}

	//================BRUSHES   
	int brushuesNum=brushes.size();
	tBSPBrush *tBrushes = new tBSPBrush[brushuesNum];

	for(int i=0;i<brushuesNum;i++){
		tBrushes[i] = brushes[i];
	}
	writeLump(
		kBrushes,
		tBrushes,
		brushes.size()
		);
	cout <<"| "<< brushes.size()<<" brushes"<<endl;
	delete [] tBrushes;
	//================ BRUSHSIDES
	int brushSidesNum = brushSides.size();
	int *tBrushesSides = new int[brushSidesNum];
	for(int i=0;i<brushSidesNum;i++){
		tBrushesSides[i] = brushSides[i];
	}
	writeLump(
		kBrushSides,
		tBrushesSides,
		brushSides.size()
		);

	cout <<"| "<< brushSides.size()<<" brushSides"<<endl;
	delete [] tBrushesSides;

	//===============NODES
	writeNodes();    
	//===============LEAFS

	int leafsNo = leafs.size();
	tBSPLeaf *leafsToWrite = new tBSPLeaf[leafsNo];
	leafsIter = leafs.begin();
	leafsEnd = leafs.end();

	int leafId=0;
	while(leafsIter!=leafsEnd){
		leafsToWrite[leafId++] = *leafsIter;
		++leafsIter;
	}

	writeLump(
		kLeafs,
		leafsToWrite,
		leafsNo
		);
	cout <<"| "<< leafsNo<<" leafs"<<endl;
	delete [] leafsToWrite;

	//============BRUSHES INDEX FOR LEAFS
	int leafsToBrushesNum = brushesIndexingForLeafs.size();
	int *tLeafsBrushesIndirection = new int[leafsToBrushesNum];

	for(int i=0;i<leafsToBrushesNum;i++)
		tLeafsBrushesIndirection[i] = brushesIndexingForLeafs[i];

	writeLump(
		kLeafBrushes,
		tLeafsBrushesIndirection,
		brushesIndexingForLeafs.size()
		);   

	cout <<"| "<< brushesIndexingForLeafs.size() <<" leafs to brushes reindirection"<<endl;
	delete [] tLeafsBrushesIndirection;

	////=================FACES POLYGONS

	int portalFacesNum = portalFaces.size();
	int facesNum = faces.size();
	int verticesNum = vertices.size();

	int theLastFacesNo = facesNum + portalFacesNum;
	vector<tBSPFace> theLastFaces(theLastFacesNo);

	for(int i=0;i<facesNum;i++){
		theLastFaces[i] = faces[i];
	}
	for(int i=0;i<portalFacesNum;i++){
		theLastFaces[facesNum+i] = portalFaces[i];
		theLastFaces[facesNum+i].vertexIndex += verticesNum;
	}

	writeLump(kLeafFaces, &theLastFaces[0], theLastFacesNo );
	cout <<"| "<< theLastFacesNo << " faces"<<endl;

	//=============VERTICES
	int portalVerticesNum = portalVertices.size();

	int theLastVerticesNo = verticesNum + portalVerticesNum;
	vector<tBSPVertex> tVertices(theLastVerticesNo);

	for(int i=0;i<verticesNum;i++)
		tVertices[i] = vertices[i];
	for(int i=0;i<portalVerticesNum;i++)
		tVertices[i+verticesNum] = portalVertices[i];

	//mergeVertices(tVertices, theLastVerticesNo);
	writeLump(kVertices, &tVertices[0], theLastVerticesNo);
	cout <<"| "<< theLastVerticesNo << " vertices"<<endl;

    ///================Neighbors
    neighbors.resize(theLastVerticesNo, -1);

    vector<int> verticesReorder;
    vector<CVector3> vertsOriginal;
    for(int i=0;i<theLastVerticesNo;i++)
        vertsOriginal.push_back(tVertices[i].vPosition);

    CTimerAdvanced timer;
    timer.start();

    if(mFindNeighbors){
#if 0
        findUniqueVertices(vertsOriginal, verticesReorder);
        int icount;
        int incount;
        icount = 0;
        for(vector<tBSPFace>::iterator i = theLastFaces.begin() ; i != theLastFaces.end() ; ++i, ++icount){
            tBSPFace &face = *i;
            for(int j = 0 ; j < face.numOfVerts ; ++j){
                int v1 = verticesReorder[face.vertexIndex + j];
                int v2 = verticesReorder[face.vertexIndex + ((j + 1) % face.numOfVerts)];

                incount = icount + 1;
                for(vector<tBSPFace>::iterator in = i + 1 ; in != theLastFaces.end() ; ++in, ++incount){
                    tBSPFace &facein = *in;
                    for(int k = 0 ; k < facein.numOfVerts ; ++k){
                        int n1 = verticesReorder[facein.vertexIndex + k];
                        int n2 = verticesReorder[facein.vertexIndex + ((k + 1) % facein.numOfVerts)];
                        if(    (v1 == n2 && v2 == n1)
                            || (v2 == n2 && v1 == n1)
                            ){
                                if(neighbors[face.vertexIndex + j] == -1)
                                    neighbors[face.vertexIndex + j] = incount;
                                else
                                    neighbors[face.vertexIndex + j] = -2;
                                if(neighbors[facein.vertexIndex + k] == -1)
                                    neighbors[facein.vertexIndex + k] = icount;
                                else
                                    neighbors[facein.vertexIndex + k] = -2;
                            }
                    }
                }
            }
        }
#else
        findUniqueVertices(vertsOriginal, verticesReorder);
        int icount;
        int incount;
        icount = 0;
        
        typedef CSparseVector<PolyEdge> CharVector;
        vector<CharVector> conn(theLastVerticesNo, CharVector(PolyEdge()));
        for(vector<tBSPFace>::iterator i = theLastFaces.begin() ; i != theLastFaces.end() ; ++i, ++icount){
            tBSPFace &face = *i;
            for(int j = 0 ; j < face.numOfVerts ; ++j){
                int v1 = verticesReorder[face.vertexIndex + j];
                int v2 = verticesReorder[face.vertexIndex + ((j + 1) % face.numOfVerts)];
                PolyEdge &ab = conn[v1][v2];
                PolyEdge &ba = conn[v2][v1];
                if(ab.edge >= 0){
                    continue;
                }
                ab.edge = face.vertexIndex + j;
                ab.poly = icount;
                if(ba.edge >= 0){
                    if(neighbors[ba.edge] == -1)
                        neighbors[ba.edge] = icount;
                    else
                        neighbors[ba.edge] = -2;
                    if(neighbors[ab.edge] == -1)
                        neighbors[ab.edge] = ba.poly;
                    else
                        neighbors[ab.edge] = -2;
                }
            }
        }
#endif
    }
#ifdef DEBUG_NEIGH
    cout << "++ Neighbors time : " << timer.stop() << endl;
#endif
#if 0
    icount = 0;
    for(vector<tBSPFace>::iterator i = theLastFaces.begin() ; i != theLastFaces.end() ; ++i, ++icount){
        tBSPFace &face = *i;
        cout << "Face " << icount << endl;
        for(int j = face.vertexIndex ; j < face.vertexIndex + face.numOfVerts ; ++j){
            cout << neighbors[j] << ", ";
        }
        cout << endl;
    }
#endif
    writeLump(kNeighbors, &neighbors[0], neighbors.size());

	///================AREAS
	writeLump(kAreas, &tareas[0], tareas.size());
	cout <<"| "<< tareas.size()<<" areas"<<endl;

	//=============AREAS TO AREAPORALS
	writeLump(
		kAreasToPortals,
		&areaPortalIndirection[0],
		areaPortalIndirection.size()
		);
	cout <<"| "<< areaPortalIndirection.size()<<" areas to portals reindirection"<<endl;    

	//============AREAPORTALS

	writeLump(
		kAreaPortals,
		&portals[0],
		portals.size()
		);
	cout <<"| "<< portals.size()<<" portals"<<endl;    
	//delete portals;

	///=============ELEMENTS
	writeLump(kElements, &elements[0],elements.size());
	cout <<"| "<< elements.size() << " elements"<<endl;           
}

const int hashSize = 1024;
unsigned int hashValue(const CVector3 &v){
    int hash = (int)(fabs(v.x)) + (int)(fabs(v.y)) + (int)(fabs(v.z)) / 64;
    hash &= hashSize - 1;
    return hash;
}

void findUniqueVertices(const vector<CVector3> &original, vector<int> &reorder){
    typedef list<unsigned int> TBucket;
    vector<TBucket> vertexHash;
    vertexHash.resize(hashSize);

#ifdef DEBUG_NEIGH
    cout << "++ Using hash table with size : "<<hashSize<<endl;
#endif

    reorder.clear();
    reorder.resize(original.size(), -1);

    vector<CVector3>::const_iterator i;
    unsigned int count = 0;
    for(i = original.begin() ; i != original.end() ; ++i, ++count){
        unsigned int hash = hashValue(*i);
        vertexHash[(hash - 1) & (hashSize - 1)].push_back(count);
        vertexHash[hash].push_back(count);
        vertexHash[(hash + 1) & (hashSize - 1)].push_back(count);
    }
    vector<TBucket>::iterator bi;
    unsigned int maxSize = 0;
    for(bi = vertexHash.begin() ; bi != vertexHash.end() ; ++bi){
        if((*bi).size() > maxSize){
            maxSize = (*bi).size();
        }
    }
#ifdef DEBUG_NEIGH
    cout << "++ Vertices : " << original.size() << " - Max in bucket : " << maxSize << endl;
#endif
    for(bi = vertexHash.begin() ; bi != vertexHash.end() ; ++bi){
        TBucket &bucket = *bi;
        for(list<unsigned int>::iterator i = bucket.begin() ; i != bucket.end() ; ++i){
            list<unsigned int>::iterator j = i;
            ++j;
            const CVector3 &v1 = original[*i];
            for(; j != bucket.end() ; ++j){
                const CVector3 &v2 = original[*j];
                if(v1 == v2){
                    int index = reorder[*i] >= 0 ? reorder[*i] : *i;
                    reorder[*j] = index;
                    assert(reorder[index] == -1);
                }
            }
        }
    }

    count = 0;
    int id = 0;
    for(vector<int>::iterator i = reorder.begin() ; i != reorder.end() ; ++i, ++id){
        if(*i < 0){
            count++;
            *i = id;
        }
    }
#ifdef DEBUG_NEIGH
    cout << "++ Unique vertices :" << count << endl;
#endif

#if 0
    cout << "-- No hashing --" << endl;
    reorder.clear();
    reorder.resize(original.size(), -1);
    int x = 0;
    for(vector<CVector3>::const_iterator i = original.begin() ; i != original.end() ; ++i, ++x){
        vector<CVector3>::const_iterator j = i;
        ++j;
        int y = x + 1;
        for(; j != original.end() ; ++j, ++y){
            if(*i == *j){
                reorder[y] = x;
            }
        }
    }
    count = 0;
    for(vector<int>::iterator i = reorder.begin() ; i != reorder.end() ; ++i){
        if(*i < 0){
            count++;
        }
    }
    cout << "Unique vertices :" << count << endl;
#endif;

}

void CBspWriter::processModelEntity(CEntity *modelEntity)
{
	vector < CBrush >::iterator brushIter,brushEnd;
	list<CBrushPlane>::iterator pBegin, pEnd;
	vector<CVertex>::iterator vIter, vEnd;
	CPolygon *polygon;
	CVector3 middle(0,0,0);
	int noVertices = 0;

	//this must first be done to have polygons to talk about
	modelEntity->callCreatePolygons();

	brushIter = modelEntity->bBegin();
	brushEnd = modelEntity->bEnd();
	while(brushIter!=brushEnd){      
		pBegin = brushIter->planesBegin();
		pEnd = brushIter->planesEnd();

		while(pBegin != pEnd ){
			polygon = pBegin->getPolygon(); 
			// till here: for every brush and brushplane of it calc the UV for its polygon
			calcUVForPolygon(polygon, polygon->getBrushPlane());
			//and add its vertices to the middle and keep the no of vertices to calc the middlepoint
			noVertices += polygon->countVertices();
			vIter = polygon->rBegin();
			vEnd = polygon->rEnd();
			while(vIter!=vEnd){
				middle += vIter->getRealCoor();
				++vIter;
			}    
			++pBegin;
		}
		++brushIter;
	}

	//this is the real middle point
	middle /= static_cast<float>(noVertices);
	//inverse
	middle = -middle;
	//move the entity to that point
	modelEntity->moveEntity(middle);
	//fix the planes of that entity //using the planemanager
	brushIter = modelEntity->bBegin();
	brushEnd = modelEntity->bEnd();
	while(brushIter!=brushEnd){      
		pBegin = brushIter->planesBegin();
		pEnd = brushIter->planesEnd();

		while(pBegin != pEnd ){
			polygon = pBegin->getPolygon();
			pBegin->setPlane( 
				pManager->getPlane( pManager->addPlane(polygon->getVertices()) ) 
				);        
			++pBegin;
		}
		++brushIter;
	}

	//make it as it was
	middle = -middle;

	//give the origin to the entity
	char origin[64];
	sprintf( origin, "%f %f %f", middle.x, -middle.z, middle.y);
	modelEntity->setProperty("origin", origin);
}

void CBspWriter::writeEntities(){
	string entitiesString ="";
	int entitiesId=0;
	int entitiesNo = loader->countEntities();
	CEntity *entity;
	int modelId=0;
	while(entitiesId<entitiesNo){
		entity = loader->getEntity(entitiesId++);


		if(entity->isModelEntity()){
			/*entitiesString +=" \"model_id\" \"";
			entitiesString+=modelId;
			entitiesString+="\" ";
			cout << "MODEL FOUND:"<<endl;
			cout << entity->asString()<<endl;;*/
			char val[128];
			sprintf(val, "%d", modelId);
			entity->setProperty("model_id", val);
			modelId++;
		}

		entitiesString+=entity->asString();
	}
	writeLump(
		kEntities,
		entitiesString.c_str(),
		entitiesString.size()+1
		);

	cout <<"| "<< modelId << " model entities"<<endl;    
}

void CBspWriter::addBrushPolygons(CBrush *brush, 
								  vector<tBSPFace> *faces, 
								  vector<tBSPElement> *elements,
								  vector<tBSPVertex> *vertices, 
								  bool calcUV,
								  bool useMergePolygon, 
								  bool useTJ)
{
	list<CBrushPlane>::iterator bpIter, bpEnd;
	bpIter = brush->planesBegin();
	bpEnd = brush->planesEnd();
	CShaderFlags *flags;
	CBrushPlane *father;
	while(bpIter!=bpEnd){
		//  if(bpIter->getPolygon())
		//here we must take the father
		father = bpIter->getFather(); 
		flags = father->getShader();
		if(flags->isKeep()){
			if(!useMergePolygon)
				addPolygon(father->getPolygon(), faces, elements, vertices, WRITE_ELEMENTS, calcUV, useTJ);
			else{
				if(father->getMerged())
					addPolygon(father->getMerged(), faces, elements, vertices, WRITE_ELEMENTS, calcUV, useTJ);  
			}
		}  
		++bpIter;
	}
}


void CBspWriter::addMergePolygon(CPolygon *polygon, 
								 vector<tBSPFace> *faces, 
								 vector<tBSPElement> *elements,
								 vector<tBSPVertex> *vertices, 
								 bool writeElements, 
								 bool calcUV, 
								 bool usetj)
{
	CPolygon *a = polygon->getBrushPlane()->getFather()->getMerged();
	if(a)
		addPolygon(a, faces, elements, vertices, writeElements, calcUV,usetj);
}

void CBspWriter::addPolygon(CPolygon *polygon, 
							vector<tBSPFace> *faces, 
							vector<tBSPElement> *elements,
							vector<tBSPVertex> *vertices, 
							bool writeElements, 
							bool calcUV, 
							bool usetj)
{
	float tess = polygon->getBrushPlane()->getShader()->getTessSize();
	if(tess > 0){
		list<CPolygon *> polys;
		list<CPolygon *>::iterator i; 
		
		polygon->appendTessPolygons(&polys);

		for(i = polys.begin() ; i != polys.end() ; ++i){
			addPolygon_tess(*i, faces, elements, vertices, writeElements, calcUV, usetj);
		}
		for(i = polys.begin() ; i != polys.end() ; ++i){
			delete *i;
		}
	} else {
        list<CPolygon *> polys;
        list<CPolygon *>::iterator pi;

        polygon->getBoundBrokenPolygons(mLightBounds, &polys);

        if(polys.size() == 1){
            addPolygon_tess(polygon, faces, elements, vertices, writeElements, calcUV, usetj);
        } else {
            for(pi = polys.begin() ; pi != polys.end() ; ++pi){
                addPolygon_tess(*pi, faces, elements, vertices, writeElements, calcUV, usetj);
            }
            for(pi = polys.begin() ; pi != polys.end() ; ++pi){
			    delete *pi;
		    }
        }
	}
}

void CBspWriter::addPolygon_tess(CPolygon *polygon, 
							vector<tBSPFace> *faces, 
							vector<tBSPElement> *elements,
							vector<tBSPVertex> *vertices, 
							bool writeElements, 
							bool calcUV, 
							bool usetj)
{
	tBSPFace newFace;
	if(!polygon->getBrushPlane())
		return;
	CBrushPlane *mBrushPlane = polygon->getBrushPlane()->getFather();
	//if(calcUV)
	//  calcUVForPolygon(polygon, mBrushPlane);        
	//add the elements
	int thisVertsNo = polygon->countVertices();
	if(writeElements){
		int elementsNo = elements->size();
		newFace.startElement=elementsNo;
        assert(newFace.startElement >= 0);
		assert(thisVertsNo>2);
		for(int el=0;el<(thisVertsNo-2);el++,elementsNo++){
			tBSPElement element;
			element.verts[0]=0;
			element.verts[1]=2+el;
			element.verts[2]=1+el;
			elements->push_back(element);
		}

		newFace.elementsSize=elements->size()-newFace.startElement;
        assert(newFace.elementsSize >= 0);
	}
	else{
		newFace.startElement=-1;
		newFace.elementsSize=-1;
	}

	//add the vertices
	int verticesNo = vertices->size();
	newFace.vertexIndex=verticesNo;
	tBSPVertex vertex;
	CVertex v;
	for(vector<CVertex>::iterator vi=polygon->getVertices().begin();
		vi!=polygon->getVertices().end();vi++){

			if(!usetj)
				v =*vi;
			else{
				//assert(vi->pvId()!=-1);
				v = tj->getVertex(vi->pvId());
			}
			if(calcUV){
				if(mBrushPlane){
					calcUVForVertex(v,mBrushPlane);
				}
				else{
					calcUVForVertex(v,polygon->getBrushPlane());
				}
			}
			vertex.vTextureCoord[0] = v.getUV(0);
			vertex.vTextureCoord[1] = v.getUV(1);

			for(int index=0;index<3;index++)
				vertex.vPosition[index]=v.getXYZ(index);

			CVector3 planeNorma;
			if(!mBrushPlane)
				planeNorma=polygon->getPlane()->getNorma();
			else
				planeNorma=mBrushPlane->getPlane()->getNorma();
            vertex.vNormal = planeNorma;
			vertices->push_back(vertex);
		}
		newFace.numOfVerts=vertices->size() - newFace.vertexIndex;
		//if(fac<startAreaPortalFace)        
		int planeID;
		if(!mBrushPlane){
			planeID=pManager->getWrittingPlaneId(polygon->getPlane()->getPlaneId());
			newFace.textureID = polygon->getShaderId();            
		}
		else{
			planeID=pManager->getWrittingPlaneId(mBrushPlane->getPlane()->getPlaneId());
			newFace.textureID = mBrushPlane->getShader()->getShaderId();
		}

		newFace.normal=planeID;

		faces->push_back(newFace);
}

vector<int> *CBspWriter::writeBrush(CBrush &brush, vector<tBSPBrush> *brushes, vector<int> *brushSides)
{
	tBSPBrush newBrush;
	newBrush.brushSide = brushSides->size();
	//newBrush.numOfBrushSides = brush.getBrushPlanesNo();
	brushSides->reserve(newBrush.brushSide+brush.getBrushPlanesNo());
	brushSides->resize(newBrush.brushSide+brush.getBrushPlanesNo());
	int index=0;
	newBrush.textureID = -1;
	list<CBrushPlane>::iterator bpIter, bpEnd;
	bpIter = brush.planesBegin();
	bpEnd = brush.planesEnd();
	while(bpIter!=bpEnd){
		(*brushSides)[newBrush.brushSide+index] = pManager->getWrittingPlaneId(bpIter->getPlane()->getPlaneId());
		//newBrush.textureID = bpIter->getShader()->getShaderId();
		++bpIter;
		index++;
	}
	newBrush.numOfBrushSides = brushSides->size()-newBrush.brushSide;
	brushes->push_back(newBrush);
	return brushSides;

}
void CBspWriter::writePlanes()
{
	pManager = loader->getPlaneManager();
	vector<CPlane *>::iterator pIter, pEnd;

	int planesNo=pManager->countWritePlanes();

	int planeIndex=0;
	pIter = pManager->writePlanesBegin();
	pEnd = pManager->writePlanesEnd();

	tBSPPlane *planes=new tBSPPlane[planesNo];
	CPlane *cplane;
	while(pIter!=pEnd){
		cplane = *pIter;
		planes[planeIndex].d = cplane->getDistanse();
		planes[planeIndex].normal = cplane->getNorma();      
		planes[planeIndex].factor = cplane->getFactor();
		planes[planeIndex].onAxe = cplane->getOnAxe();
		++pIter;
		++planeIndex;
	}
	writeLump(
		kPlanes,
		planes,
		planesNo
		);
	cout <<"| "<< planesNo<<" planes"<<endl;
	delete []planes;
}

void CBspWriter::writeShaders(){
	int texturesNo=loader->countShaders();
	int texturesIndex;
	tBSPTexture *textures = new tBSPTexture[texturesNo];
	texturesIndex=0;
	vector<string>::iterator it, siEnd;
	it = loader->sBegin();
	siEnd = loader->sEnd();


	while(it!=siEnd){
		memset(textures[texturesIndex].strName, 0, 64);
		strcpy(textures[texturesIndex].strName, "textures/");
		strcpy(
			textures[texturesIndex].strName,    
			it->c_str()
			);
		//textures[texturesIndex].flags=-1;
		//textures[texturesIndex].contents=-1;
		++texturesIndex;
		it++;
	}


	writeLump(
		kTextures,
		textures,
		texturesNo
		);  
	cout <<"| "<< texturesNo<< " shader names" <<endl;
	delete []textures; 
}

void CBspWriter::writeMemData()
{
	FILE *f = fopen(filePath.c_str(), "w+b");
	if(!f){
		cout <<" Error writting file: "<<strerror(errno)<<endl;
		return;
	}

	int bytesWritten = fwrite(&memData[0], 1, memData.size(), f);
	fflush(f);
	fclose(f);
	cout << "file "<< filePath<<" written"<<endl;
	cout << bytesWritten<<" bytes"<<endl;
	//    ofstream out(filePath.c_str(), ios::out | ios::trunc);


	//    out.write(&memData[0], memData.size());
	//    out.close();
	//    cout << "file written"<<endl;    

	/*   HANDLE h=CreateFile(filePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ,
	0, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	int r = GetLastError();
	if(r!=0){
	cout << "create new file" <<endl;
	h = CreateFile(filePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ,
	0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);	
	r = GetLastError();
	}
	else
	cout << "ovewritting file"<<endl;

	DWORD size;
	WriteFile(h, &memData[0], memData.size(), &size, 0);//prelast is size
	cout << "Wrote " << size << " of " << memData.size() << endl ;
	cout << "Handle" << h <<" "<< filePath << endl ;
	FlushFileBuffers(h);
	CloseHandle(h);*/
}



/*template < class T > 
void CBspWriter::(const int lumpNo, const T *data, const int dataNo){
int bytesNo=dataNo*sizeof(T);
int offset = memData.size();
memData.reserve(offset+bytesNo);
memData.resize(offset+bytesNo);

T *templData = (T*)&memData[offset];

memcpy(templData,data, bytesNo);

tBSPLump *lumpP = (tBSPLump*)&memData[sizeof(tBSPHeader)+lumpNo*sizeof(tBSPLump)];
lumpP->offset=offset;
lumpP->length=bytesNo;           
}*/



void CBspWriter::calcUVForVertex(CVertex &v, CBrushPlane *bplane){
	v.setU((bplane->getXOff() +bplane->getXTextureMatrix() *v.getRealCoor()));
	v.setV((bplane->getYOff() + bplane->getYTextureMatrix() *v.getRealCoor()));
}

void CBspWriter::calcUVForPolygon(CPolygon *polygon, CBrushPlane *brushPlane)
{
	float mins[2], maxs[2];
	vector<CVertex>::iterator vIter, vEnd;

	mins[0] = mins[1] = huge;
	maxs[0] = maxs[1] = -huge;

	CBrushPlane *bplane;
	if(brushPlane)
		bplane = brushPlane;
	else
		bplane = polygon->getBrushPlane();

	vIter = polygon->rBegin();
	vEnd = polygon->rEnd();

	while(vIter!=vEnd){
#ifdef USE_SNAP
		for (int k = 0; k < 3; k++) {
			vIter->setXYZ(k,SNAP_INT_TO_FLOAT *floor(vIter->getXYZ(k)*SNAP_FLOAT_TO_INT + 0.5));
		}
#endif

#ifdef USE_ROUND
		for (int k = 0; k < 3; k++) {
			vIter->setXYZ(k,floor(vIter->getXYZ(k)));
		}
#endif
		//CVector3 f = vIter->getRealCoor();
		calcUVForVertex(*vIter, bplane);
		for (int k = 0; k < 2; k++) {
			if (vIter->getUV(k) < mins[k]) {
				mins[k] = vIter->getUV(k);
			} if (vIter->getUV(k) > maxs[k]) {
				maxs[k] = vIter->getUV(k);
			}
		}
		++vIter;
	}
}



void CBspWriter::rewriteEntities()
{
	cout << "Reading bsp from " << filePath << endl;
	FILE *f = fopen(filePath.c_str(), "rb");
	if(!f){
		cout <<"Error reading file: "<<strerror(errno)<<endl;
		return;
	}

	tBSPLump entitiesLump;

	fseek(f, sizeof(tBSPHeader), SEEK_SET);
	fread(&entitiesLump, sizeof(tBSPLump), 1, f);

	memData.reserve(entitiesLump.offset);
	memData.resize(entitiesLump.offset);
	char *templData = (char*)&memData[0];
	char *data = new char[entitiesLump.offset];
	fseek(f,0,SEEK_SET);

	fread(data, 1, entitiesLump.offset, f);

	memcpy(templData,data, entitiesLump.offset);
	fflush(f);
	fclose(f);
	//write lump 0   (Entities)
	/*  string entitiesString ="";
	int entitiesNo=loader->countEntities()-1;
	while(entitiesNo>=0){
	entitiesString+=loader->getEntity(entitiesNo--)->asString();
	}
	writeLump(
	kEntities,
	entitiesString.c_str(),
	entitiesString.size()+1
	);*/
	//cout << loader->countEntities()<<" the new entities"<<endl;
	writeEntities();
	writeMemData();
}


void CBspWriter::mergeVertices(tBSPVertex *tVertices, const int theLastVerticesNo)
{
	float d;
	float POINTS_MERGE_EPSILON = 1.0f;
	for(int i=0;i<theLastVerticesNo-1;i++){ 
		for(int j=i+1;j<theLastVerticesNo;j++){
			d = fabs(tVertices[j].vPosition[0] - tVertices[i].vPosition[0]);
			if(d>=POINTS_MERGE_EPSILON)
				continue;
			d = fabs(tVertices[j].vPosition[1] - tVertices[i].vPosition[1]);
			if(d>=POINTS_MERGE_EPSILON)
				continue;
			d = fabs(tVertices[j].vPosition[2] - tVertices[i].vPosition[2]);
			if(d>=POINTS_MERGE_EPSILON)
				continue;
			/*
			CVector3 d(tVertices[j].vPosition[0],
			tVertices[j].vPosition[1],
			tVertices[j].vPosition[2]);
			d -= CVector3(tVertices[i].vPosition[0],
			tVertices[i].vPosition[1],
			tVertices[i].vPosition[2]);
			if(d.length() > 0.5)
			continue;
			>>>>>>> .r37*/
			tVertices[j].vPosition[0] = tVertices[i].vPosition[0];
			tVertices[j].vPosition[1] = tVertices[i].vPosition[1];
			tVertices[j].vPosition[2] = tVertices[i].vPosition[2];            
		}
	}
}

