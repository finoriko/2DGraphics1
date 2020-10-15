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
		// 틀 별명. 코드를 짧게 하고 싶다.
		typedef unsigned U4; // 바이트 무부호 별명
		typedef unsigned short U2; //2바이트 무부호 별명

		//설정 상수군
		const U4 MAX_NORMAL_BLOCK_SIZE = 128 * 1024; // 더 이상은 Virtual Alloc에서 직접 확보
		const U4 HEAP_REGION_SIZE_BIT = 24; // OS에서 한번에 집어넣는 영역 크기(예를 들어 20으로 1MB, 24로 16MB)
		const U4 ALIGN = 8; // 알라인 사이즈 4, 8, 16 쯤에서 고르자.4를 선택할 때는 double이 없음을 보장할 것.
		//도출 상수군
		const U4 HEAP_REGION_SIZE = ( 1 << HEAP_REGION_SIZE_BIT );
		const U4 TABLE_NUMBER = (HEAP_REGION_SIZE_BIT - 3) * 4;// 빈 리스트 상자의 수(4 마다 배가 되므로 사이즈 비트수의 4배이지만 4, 8, 12, 16으로 시작하기 때문에, 여러가지 있어 -3)

		// 블록 플래그
		/*
		큰 블록인지를 알고 싶은 것은 해방 시. 해방 시이기 때문에 사용 중으로 정해져 있다.
		한편, 하늘인지 어떤지를 알고 싶은 것은 Heap내라는 것을 알고 나서이기 때문에, 같은 비트를 돌려 사용한다.
		*/

		const U4 FLAG_LARGE_BLOCK = (1 << 0); // 큰 블록입니다.
		const U4 FLAG_EMPTY = (1 << 0); // 비어있습니다.
		const U4 FLAG_PREV_EMPTY = (1 << 1); // 앞의 블록은 비어 있습니다
		const U4 SIZE_MASK = ~(FLAG_PREV_EMPTY | FLAG_EMPTY);

		#ifdef USE_DEBUG_INFO

		const U2 FILE_INDEX_MAX = 65521; //65536 이하 최대 소수
		const U2 FILE_INDEX_UNKNOWN = FILE_INDEX_MAX - 1;
		const char* gFileNames[FILE_INDEX_MAX];

		U2 getFileNameIndex(const char* p) {
			if (!p) {
				return FILE_INDEX_UNKNOWN;
			}
			//해시값계산
			ptrdiff_t address = p - static_cast<const char*>(0);
			U2 h = static_cast<U2>(address % FILE_INDEX_MAX);
			//해시값을 첨자 초기값으로 인덱스겟
			for (U2 i = h; i < FILE_INDEX_UNKNOWN; ++i) {
				if (!gFileNames[i] || (gFileNames[i] == p)) {
					gFileNames[i] = p;
					return i;
				}
			}
			//이쪽으로 올 것 같으면 한참 끝나네.아마 만족한 속도로 움직이지 않을 거야.
			for (U2 i = 0; i < h; ++i) {
				if (!gFileNames[i] || (gFileNames[i] == p)) {
					gFileNames[i] = p;
					return i;
				}
			}
			STRONG_ASSERT(false && "MemoryManager : FileName Table Full!");
			return FILE_INDEX_UNKNOWN;
		}

		#endif //DEBUG_INFO
		// 파일명 해시
		/*
		디버그 정보용 파일명은 글로벌 변수에 정리해 둔다.4바이트를 아끼기 때문이다.
		*/

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
