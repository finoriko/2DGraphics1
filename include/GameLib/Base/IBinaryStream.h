#ifndef INCLUDED_GAMELIB_IBINARYSTREAM_H
#define INCLUDED_GAMELIB_IBINARYSTREAM_H

#include "GameLib/Base/RefString.h"

namespace GameLib {

	class RefString;

	///바이너리 파일 읽기를 편하게 하기 위한 클래스
	/*!
	기본적으로 복사를 하지 않는다.따라서 원래의 문자열은 유지될 필요가 있다.
	*/
	class IBinaryStream {
	public:
		IBinaryStream(const char*);
		IBinaryStream(const char*, int size);
		IBinaryStream(const RefString&);
		~IBinaryStream();
		/// 반환값은 읽을 수 있었던 바이트 수
		int read(char*, int size);
		// 각종 임베디드 정수형용 버전.성공 실패는 인수로 돌아가지만 내주지 않으면 아무것도 하지 않는다.
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