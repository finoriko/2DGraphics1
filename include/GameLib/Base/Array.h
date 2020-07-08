#ifndef INCLUDED_GAMELIB_ARRAY_H
#define INCLUDED_GAMELIB_ARRAY_H

namespace GameLib{

//해제를 잘 잊어버리는 분을 위한 자동 해제 배열 템플릿
template< class T > class Array{
public:
	///디폴트 사이즈는 0
	Array();
	///초기 사이즈입니다.모든 요소는 기본 생성자로 초기화됩니다.
	explicit Array( int size );
	//초기 사이즈 초기값 전달
	explicit Array( int size, const T& );
	~Array();
	///나중에 확보。축소는 가능하지만、확대는 불가。일단clear()할것。
	void setSize( int size );
	///사이즈 가져오기
	int size() const;
	///클리어
	void clear();
	///添え字(const)//첨자
	const T& operator[]( int i ) const;
	///添え字(非const)//비첨자
	T& operator[]( int i );
private:
	void operator=( const Array& ); //대입금지
	Array( const Array& ); //복사 생성자 금지

	T* mElements;
	int mSize;
};

} //namespace GameLib
#include "GameLib/Base/Impl/ArrayImpl.h" //中身はこの中

#endif

