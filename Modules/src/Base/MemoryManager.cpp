#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#endif





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
