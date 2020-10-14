#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#endif

#include <cstring> //sprintf 등등.쓰기 싫지만 어쩔 수 없지.
#include <cstdio>
#include <locale>
#include "GameLib/GameLib.h"
#include "GameLib/Base/MemoryManager.h"

//#define STRONG_DEBUG //만들고 있는 사람용 디버그

#ifndef NDEBUG //디버그 때만 디버그 정보
#define USE_DEBUG_INFO
#endif

// 이 파일 안에는 C++ 라이브러리를 사용하지 않고 있다.
//왜냐하면, new로부터 불려지는 장소인데, 안에서 new 하는 것과 같은 일을 하면 무한 재귀에 빠지기 때문이다.
//C++의 라이브러리는 안에서 마음대로 new하므로 어느 것을 사용해야 한다고 판단하기 어렵다.
//C 언어의 함수는 기본적으로 사용하지 말라고 썼는데, 아무래도 사용해야 할 때라는 것은 있다.

namespace GameLib
{
	namespace//어느 정도 안정 동작할 때까지 떼어 둔다.몸부림치기 힘들어 죽겠어.
	{
		using namespace std;

		Impl gImpl;
	}
	MemoryManager MemoryManager::instance() {
		return MemoryManager();
	}

	void MemoryManager::write(const char* filename) {
		gImpl.write(filename);
	}

	int MemoryManager::totalSize() const {
		return gImpl.totalSize();
	} //namespace {}
} //namespace gamelib

using namespace GameLib;

#ifdef USE_DEBUG_INFO
void* operator new(size_t size, const char* filename, int line) {
	unsigned debugInfo = (line << 16) | getFileNameIndex(filename);
	return gImpl.allocate(size, debugInfo);
#else
void* operator new(size_t size, const char*, int) {
	return gImpl.allocate(size);
#endif
}

#ifdef USE_DEBUG_INFO
void* operator new[](size_t size, const char* filename, int line) {
	unsigned debugInfo = (line << 16) | getFileNameIndex(filename);
	return gImpl.allocate(size, debugInfo);
#else
void* operator new[](size_t size, const char*, int) {
	return gImpl.allocate(size);
#endif
}

void operator delete(void* p, const char*, int) {
	gImpl.deallocate(p);
}

void operator delete[](void* p, const char*, int) {
	gImpl.deallocate(p);
}

void* operator new(size_t size) {
	return gImpl.allocate(size);
}

void* operator new[](size_t size) {
	return gImpl.allocate(size);
}

void operator delete(void* p) {
	gImpl.deallocate(p);
}

void operator delete[](void* p) {
	gImpl.deallocate(p);
}
