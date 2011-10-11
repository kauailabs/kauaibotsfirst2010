/*
 * ISynchronizer.h
 *
 *  Created on: Aug 22, 2011
 *      Author: smarlin
 */

#ifndef ISYNCHRONIZER_H_
#define ISYNCHRONIZER_H_
#include "Mutex.h"
class ISynchronizer {
public:
	virtual Mutex* getMutexPtr() = 0;
};

#endif /* ISYNCHRONIZER_H_ */
