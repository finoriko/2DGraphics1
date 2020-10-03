#ifndef INCLUDED_GAMELIB_COMPRESSOR_H
#define INCLUDED_GAMELIB_COMPRESSOR_H

#include "GameLib/Base/Array.h"

namespace GameLib {

	/// 압축 전개 클래스
	/*
	그 자리 전개를 하려면 compress()로 되돌아온 get InPlace Decompression Buffer Size
	만큼 버퍼를 확보해, 압축 데이터를 뒤에 채워 격납하고, decompress를 부른다.
	*/
	class Compressor {
	public:
		///압축
		static void compress(
			Array< char >* compressed,
			int* inPlaceDecompressionBufferSize, //그 자리 전개시 필요한 버퍼 크기
			const char* data,
			int size);
		///전개. 전개 실패는 outSize가 올바른지 체크하라.outSize가 너무 크면 메모리를 파괴하여 치명상.
		static void decompress(
			char* outData,
			int* outSize,
			const char* data,
			int size);
	};

} //namespace GameLib

#endif
