#ifndef INCLUDED_GAMELIB_GAMELIB_H
#define INCLUDED_GAMELIB_GAMELIB_H

namespace GameLib {
	enum Exception {
		EXCEPTION_IGNORE,
		EXCEPTION_EXIT,
	};
	void halt(const char* filename, int line, const char* message);
}

#include "Base/DebugStream.h"
#include "Base/Array.h"
#include "Base/AutoPtr.h"
#include "Base/SharedPtr.h"
#include "Base/MemoryManager.h"
#include <new>

#endif