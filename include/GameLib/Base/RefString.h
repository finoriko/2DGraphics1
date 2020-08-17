#ifndef INCLUDED_GAMELIB_REFSTRING_H
#define INCLUDED_GAMELIB_REFSTRING_H

//#include <string>

namespace GameLib {
	//using namespace std;

	//참조형 문자열 클래스.
	/*!
	참조 밖에 가지지 않기 때문에, 원래의 데이터가 파기되면 매우 위험하게 된다.
	const char*에서만 불편한 경우 NULL 종단이 불가능한 경우로,
	성능이 신경 쓰이는 경우에만 사용할 것을 권장한다.
	*/
	class RefString {
	public:
		RefString();
		RefString(const char*, int size);
		explicit RefString(const char*);
		~RefString();
		void set(const char*, int size);
		void operator=(const char*);
		//	void operator=( const string& );
		char operator[](int i) const;
		const char* get() const;
		int size() const;
		void setSize(int);
		///last는 마지막 문자 그 자체. +1 하지마.last==-1이면 시작점만 바꾼다
		void clamp(int first, int last = -1);
		int find(char) const; //앞에서문자를찾아위치를돌려준다.찾아야지 - 1.
		int rfind(char) const; //나중에 문자를 찾아 위치를 반환한다.찾아야지 - 1.
		///참조처 문자열을 복사하여 포인터를 반환한다.자기 부담으로 delete할 것.
		char* copyOriginal() const;

		bool operator==(const RefString&) const;
		bool operator==(const char*) const;
		//	bool operator==( const string& ) const;
		bool operator!=(const RefString&) const;
		bool operator!=(const char*) const;
		//	bool operator!=( const string& ) const;
		bool operator<(const RefString&) const;
	private:
		const char* mString;
		int mSize;
	};

} //namespace GameLib

#endif