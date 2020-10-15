#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#endif

#include <cstring> //sprintf ���.���� ������ ��¿ �� ����.
#include <cstdio>
#include <locale>
#include "GameLib/GameLib.h"
#include "GameLib/Base/MemoryManager.h"

//#define STRONG_DEBUG //����� �ִ� ����� �����

#ifndef NDEBUG //����� ���� ����� ����
#define USE_DEBUG_INFO
#endif

// �� ���� �ȿ��� C++ ���̺귯���� ������� �ʰ� �ִ�.
//�ֳ��ϸ�, new�κ��� �ҷ����� ����ε�, �ȿ��� new �ϴ� �Ͱ� ���� ���� �ϸ� ���� ��Ϳ� ������ �����̴�.
//C++�� ���̺귯���� �ȿ��� ������� new�ϹǷ� ��� ���� ����ؾ� �Ѵٰ� �Ǵ��ϱ� ��ƴ�.
//C ����� �Լ��� �⺻������ ������� ����� ��µ�, �ƹ����� ����ؾ� �� ����� ���� �ִ�.

namespace GameLib
{
	namespace//��� ���� ���� ������ ������ ���� �д�.���θ�ġ�� ����� �װھ�.
	{
		using namespace std;
		// Ʋ ����. �ڵ带 ª�� �ϰ� �ʹ�.
		typedef unsigned U4; // ����Ʈ ����ȣ ����
		typedef unsigned short U2; //2����Ʈ ����ȣ ����

		//���� �����
		const U4 MAX_NORMAL_BLOCK_SIZE = 128 * 1024; // �� �̻��� Virtual Alloc���� ���� Ȯ��
		const U4 HEAP_REGION_SIZE_BIT = 24; // OS���� �ѹ��� ����ִ� ���� ũ��(���� ��� 20���� 1MB, 24�� 16MB)
		const U4 ALIGN = 8; // �˶��� ������ 4, 8, 16 �뿡�� ����.4�� ������ ���� double�� ������ ������ ��.
		//���� �����
		const U4 HEAP_REGION_SIZE = ( 1 << HEAP_REGION_SIZE_BIT );
		const U4 TABLE_NUMBER = (HEAP_REGION_SIZE_BIT - 3) * 4;// �� ����Ʈ ������ ��(4 ���� �谡 �ǹǷ� ������ ��Ʈ���� 4�������� 4, 8, 12, 16���� �����ϱ� ������, �������� �־� -3)

		// ��� �÷���
		/*
		ū ��������� �˰� ���� ���� �ع� ��. �ع� ���̱� ������ ��� ������ ������ �ִ�.
		����, �ϴ����� ����� �˰� ���� ���� Heap����� ���� �˰� �����̱� ������, ���� ��Ʈ�� ���� ����Ѵ�.
		*/

		const U4 FLAG_LARGE_BLOCK = (1 << 0); // ū ����Դϴ�.
		const U4 FLAG_EMPTY = (1 << 0); // ����ֽ��ϴ�.
		const U4 FLAG_PREV_EMPTY = (1 << 1); // ���� ����� ��� �ֽ��ϴ�
		const U4 SIZE_MASK = ~(FLAG_PREV_EMPTY | FLAG_EMPTY);

		#ifdef USE_DEBUG_INFO

		const U2 FILE_INDEX_MAX = 65521; //65536 ���� �ִ� �Ҽ�
		const U2 FILE_INDEX_UNKNOWN = FILE_INDEX_MAX - 1;
		const char* gFileNames[FILE_INDEX_MAX];

		U2 getFileNameIndex(const char* p) {
			if (!p) {
				return FILE_INDEX_UNKNOWN;
			}
			//�ؽð����
			ptrdiff_t address = p - static_cast<const char*>(0);
			U2 h = static_cast<U2>(address % FILE_INDEX_MAX);
			//�ؽð��� ÷�� �ʱⰪ���� �ε�����
			for (U2 i = h; i < FILE_INDEX_UNKNOWN; ++i) {
				if (!gFileNames[i] || (gFileNames[i] == p)) {
					gFileNames[i] = p;
					return i;
				}
			}
			//�������� �� �� ������ ���� ������.�Ƹ� ������ �ӵ��� �������� ���� �ž�.
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
		// ���ϸ� �ؽ�
		/*
		����� ������ ���ϸ��� �۷ι� ������ ������ �д�.4����Ʈ�� �Ƴ��� �����̴�.
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
