#ifndef INCLUDED_GAMELIB_BASE_MEMORYMANAGER_H
#define INCLUDED_GAMELIB_BASE_MEMORYMANAGER_H

namespace GameLib{

//newئ ԣԧǏԂ Ŭסݺ
class MemoryManager{
public:
	static MemoryManager instance();

	///dڸ ޲Ѣ(Ƅح; Vة Ƅ׎, ވ Vة հٶї âׂ8׎)
	void write( const char* filename = 0 );
	///ذէ Ȯڸ ߫׮(ӻڎ ߹ޠ ߫׮ Ʒǔ)
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
