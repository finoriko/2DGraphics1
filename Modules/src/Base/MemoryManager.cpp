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
