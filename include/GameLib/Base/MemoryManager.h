#ifndef INCLUDED_GAMELIB_BASE_MEMORYMANAGER_H
#define INCLUDED_GAMELIB_BASE_MEMORYMANAGER_H

namespace GameLib{

//new를 담당하는 클래스
class MemoryManager{
public:
	static MemoryManager instance();

	///정보 첫머리(파일명을 주면 파일로, 주지 않으면 디버그 출력으로)
	void write( const char* filename = 0 );
	///전체 확보 용량(내부 예약 용량 포함)
	int totalSize() const;
};

} //namespaace GameLib

void* operator new( size_t size, const char* filename, int line );
void* operator new[]( size_t size, const char* filename, int line );
void operator delete( void*, const char*, int );
void operator delete[]( void*, const char*, int );
void* operator new( size_t size );
void* operator new[]( size_t size );
void operator delete( void* );
void operator delete[]( void* );

#endif
