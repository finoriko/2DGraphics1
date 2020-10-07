#ifndef INCLUDED_GAMELIB_HASTMAPIMPL_H
#define INCLUDED_GAMELIB_HASHMAPIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/HashMap.h"
#include "GameLib/Base/PrimeNumber.h"

namespace GameLib 
{
template< class K, class V, class H > inline bool HashMap< K, V, H >::isEnd(int position) const {
	return (position == mCapacity + mTableSize);
}

template< class K, class V, class H > inline void HashMap< K, V, H >::copyTo(V* a) const {
	int p = mNexts[mCapacity];
	int idx = 0;
	while (p != mCapacity + mTableSize) {
		if (p < mCapacity) {
			a[idx] = mValues[p];
			++idx;
		}
		p = mNexts[p];
	}
}

template< class K, class V, class H > inline void HashMap< K, V, H >::copyTo(Array< V >* a) const {
	a->setSize(mSize);
	if (mSize > 0) {
		copyTo(&((*a)[0]));
	}
}


template< class K, class V, class H > inline int HashMap< K, V, H >::size() const {
	return mSize;
}

} //namespace GameLib

#endif
