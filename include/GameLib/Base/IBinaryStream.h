#ifndef INCLUDED_GAMELIB_IBINARYSTREAM_H
#define INCLUDED_GAMELIB_IBINARYSTREAM_H

#include "GameLib/Base/RefString.h"

namespace GameLib {

	class RefString;

	///���̳ʸ� ���� �б⸦ ���ϰ� �ϱ� ���� Ŭ����
	/*!
	�⺻������ ���縦 ���� �ʴ´�.���� ������ ���ڿ��� ������ �ʿ䰡 �ִ�.
	*/
	class IBinaryStream {
	public:
		IBinaryStream(const char*);
		IBinaryStream(const char*, int size);
		IBinaryStream(const RefString&);
		~IBinaryStream();
		/// ��ȯ���� ���� �� �־��� ����Ʈ ��
		int read(char*, int size);
		// ���� �Ӻ���� �������� ����.���� ���д� �μ��� ���ư����� ������ ������ �ƹ��͵� ���� �ʴ´�.
		int readInt(bool* succeeded = 0);
		unsigned readUnsigned(bool* succeeded = 0);
		short readShort(bool* succeeded = 0);
		unsigned short readUnsignedShort(bool* succeeded = 0);
		char readChar(bool* succeeded = 0);
		unsigned char readUnsignedChar(bool* succeeded = 0);

		bool isEnd() const;
		void setPosition(int = 0);
		int position() const;
		int size() const;
	private:
		const char* mBuffer;
		int mSize;
		int mPosition;
	};

} //namespace GameLib

#endif