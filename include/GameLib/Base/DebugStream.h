#ifndef INCLUDED_GAMELIB_DEBUGSTREAM_H
#define INCLUDED_GAMELIB_DEBUGSTREAM_H

//#include <string>
//using namespace std;

namespace GameLib{

class RefString;
class OStringStream;

//디버그아웃에 cout으로 내기 위한것
//여러 스레드로 동시에 불러도 된다
class DebugStream{
public:
	DebugStream();
	int precision( int );
	DebugStream& operator<<( char );
	DebugStream& operator<<( unsigned char );
	DebugStream& operator<<( int );
	DebugStream& operator<<( unsigned );
	DebugStream& operator<<( short );
	DebugStream& operator<<( unsigned short );
	DebugStream& operator<<( float );
	DebugStream& operator<<( double );
//	DebugStream& operator<<( const string& );
	DebugStream& operator<<( const char* );
	DebugStream& operator<<( char* );
	DebugStream& operator<<( const RefString& );
	DebugStream& operator<<( const OStringStream& );
	DebugStream& operator<<( DebugStream& ( *f )( DebugStream& ) );
	DebugStream& endl();
	DebugStream& hex();
	DebugStream& dec();

	void begin();
	void end();
private:
	class Impl;
	Impl* mImpl;
};

DebugStream& endl( DebugStream& );
DebugStream& hex( DebugStream& );
DebugStream& dec( DebugStream& );
extern DebugStream cout;

} //namespace GameLib

#endif
