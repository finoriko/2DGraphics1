#ifndef INCLUDED_GAMELIB_COMPRESSOR_H
#define INCLUDED_GAMELIB_COMPRESSOR_H

#include "GameLib/Base/Array.h"

namespace GameLib {

	/// ���� ���� Ŭ����
	/*
	�� �ڸ� ������ �Ϸ��� compress()�� �ǵ��ƿ� get InPlace Decompression Buffer Size
	��ŭ ���۸� Ȯ����, ���� �����͸� �ڿ� ä�� �ݳ��ϰ�, decompress�� �θ���.
	*/
	class Compressor {
	public:
		///����
		static void compress(
			Array< char >* compressed,
			int* inPlaceDecompressionBufferSize, //�� �ڸ� ������ �ʿ��� ���� ũ��
			const char* data,
			int size);
		///����. ���� ���д� outSize�� �ùٸ��� üũ�϶�.outSize�� �ʹ� ũ�� �޸𸮸� �ı��Ͽ� ġ���.
		static void decompress(
			char* outData,
			int* outSize,
			const char* data,
			int size);
	};

} //namespace GameLib

#endif
