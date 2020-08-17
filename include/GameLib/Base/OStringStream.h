#ifndef INCLUDED_GAMELIB_OSTRINGSTREAM_H
#define INCLUDED_GAMELIB_OSTRINGSTREAM_H

#include <string>

namespace GameLib {
	using namespace std;

	class RefString;

	///std :: ostring stream�� ġȯ�ϱ� ���� ��. �ӵ��� ������ �ξ� ����� �ټ� ����ǰ� �ִ�.
	/*!
	std: ostring stream���� ��������
	str()�� ����.get()���� Array�� �Ѱ� ������.
	clear()�� ���Ұ�
	����� ����.
	*/
	class OStringStream {
	public:
		OStringStream(int blockSize = 1024); // �ѹ��� new ������.Ŭ���� ���ϴ� �������� �޸𸮴� �Դ´�.
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
		template< class T > void write(const T&); //���̳ʸ� �����ϱ�
		OStringStream& endl();
		OStringStream& hex();
		OStringStream& dec();

		void get(Array< char >*) const;
		void get(string*) const;
		int size() const;
		void clear();
	private:
		OStringStream(const OStringStream&); //���� �ܽ�Ʈ���� ����
		void operator=(const OStringStream&); //���Ա���
		class Impl;
		Impl* mImpl;
	};

	OStringStream& endl(OStringStream&);
	OStringStream& hex(OStringStream&);
	OStringStream& dec(OStringStream&);

} //namespace GameLib
#include "GameLib/Base/Impl/OStringStreamImpl.h"

#endif
