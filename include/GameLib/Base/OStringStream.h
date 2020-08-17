#ifndef INCLUDED_GAMELIB_OSTRINGSTREAM_H
#define INCLUDED_GAMELIB_OSTRINGSTREAM_H

#include <string>

namespace GameLib {
	using namespace std;

	class RefString;

	///std :: ostring stream을 치환하기 위한 것. 속도에 중점을 두어 기능이 다소 희생되고 있다.
	/*!
	std: ostring stream과의 차이점은
	str()이 없다.get()으로 Array를 넘겨 꺼낸다.
	clear()로 전소거
	기능이 적다.
	*/
	class OStringStream {
	public:
		OStringStream(int blockSize = 1024); // 한번의 new 사이즈.클수록 부하는 가볍지만 메모리는 먹는다.
		~OStringStream();
		int precision(int);
		OStringStream& operator<<(char);
		OStringStream& operator<<(unsigned char);
		OStringStream& operator<<(int);
		OStringStream& operator<<(unsigned);
		OStringStream& operator<<(short);
		OStringStream& operator<<(unsigned short);
		OStringStream& operator<<(float);
		OStringStream& operator<<(double);
		OStringStream& operator<<(const string&);
		OStringStream& operator<<(const char*);
		OStringStream& operator<<(char*);
		OStringStream& operator<<(const RefString&);
		OStringStream& operator<<(OStringStream& (*f)(OStringStream&));
		void write(const char*, int size);
		template< class T > void write(const T&); //바이너리 기입하기
		OStringStream& endl();
		OStringStream& hex();
		OStringStream& dec();

		void get(Array< char >*) const;
		void get(string*) const;
		int size() const;
		void clear();
	private:
		OStringStream(const OStringStream&); //복사 콘스트럭터 금지
		void operator=(const OStringStream&); //대입금지
		class Impl;
		Impl* mImpl;
	};

	OStringStream& endl(OStringStream&);
	OStringStream& hex(OStringStream&);
	OStringStream& dec(OStringStream&);

} //namespace GameLib
#include "GameLib/Base/Impl/OStringStreamImpl.h"

#endif
