#ifndef INCLUDED_GAMELIB_REFSTRING_H
#define INCLUDED_GAMELIB_REFSTRING_H

//#include <string>

namespace GameLib {
	//using namespace std;

	//������ ���ڿ� Ŭ����.
	/*!
	���� �ۿ� ������ �ʱ� ������, ������ �����Ͱ� �ı�Ǹ� �ſ� �����ϰ� �ȴ�.
	const char*������ ������ ��� NULL ������ �Ұ����� ����,
	������ �Ű� ���̴� ��쿡�� ����� ���� �����Ѵ�.
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
		///last�� ������ ���� �� ��ü. +1 ������.last==-1�̸� �������� �ٲ۴�
		void clamp(int first, int last = -1);
		int find(char) const; //�տ������ڸ�ã����ġ�������ش�.ã�ƾ��� - 1.
		int rfind(char) const; //���߿� ���ڸ� ã�� ��ġ�� ��ȯ�Ѵ�.ã�ƾ��� - 1.
		///����ó ���ڿ��� �����Ͽ� �����͸� ��ȯ�Ѵ�.�ڱ� �δ����� delete�� ��.
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