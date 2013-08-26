/***************************************************************************
                          cache.h  -  description
                             -------------------
    begin                : Sat Jul 5 2002
    copyright            : (C) 2003 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#ifndef CACHE_H
#define CACHE_H

#include <list>
//#include <hash_map>

template<class KeyType, class ValueType, class Generator>
class CCache {
public:
    CCache(){
    };
	CCache(S32 size, Generator &g){
		mSize = size;
		mGen = g;
		mAccesses = mMisses = 0;
	};
	~CCache(){

	};

	ValueType &access(const KeyType &key){
		mAccesses++;
		typename CacheList::iterator i;
		for(i=mList.begin() ; i!=mList.end() ; i++){
			if((*i).first == key){
				//mGen(key, (*i).second);
				return (*i).second;
			}
		}
		mMisses++;
		return create(key);
	};

	ValueType &create(const KeyType &key){
		ValueType t;
        //static S32 i=0;
        //gcon.printf("* %f %% %d\n", (F32)mMisses/(F32)mAccesses,i++);
		mList.push_front(KeyValue(key, t));
		mGen(key, (*mList.begin()).second);

		if(mList.size() > mSize)
			mList.pop_back();
		return (*mList.begin()).second;
	};

	void purgeData(){
		mList.clear();
	}

protected:
	Generator mGen;
	typedef pair<KeyType, ValueType> KeyValue;
	typedef list<KeyValue> CacheList;

	CacheList mList;

	S32 mAccesses;
	S32 mMisses;
	S32 mSize;
};

#endif


