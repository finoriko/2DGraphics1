#ifndef INCLUDED_GAMELIB_GRAPHICS_TEXTURE_H
#define INCLUDED_GAMELIB_GRAPHICS_TEXTURE_H

namespace GameLib {
	namespace PseudoXml {
		class ConstElement;
	}
	namespace Graphics {

		class Texture {
		public:
			///엘리먼트에서 생성.path가 있으면 파일 이름에 추가한다.
			static Texture create(PseudoXml::ConstElement, const char* path = 0);
			// 빈 텍스처
			static Texture create(int width, int height, bool createMipChain);
			///TGA 혹은 DDS. 확장자로 판별한다.
			static Texture create(const char* filename);
			///TGA 혹은 DDS의 내용물을 전달한다.처음에 DDS라고 써 있으면 DDS로 간주하고, 그 이외에는 TGA로 간주한다.
			static Texture create(const char* fileData, int fileSize);
			///이름 가져오기(가짜xml에서 로드하면 그 이름.다른 것은 비어있다).
			const char* name() const;

			///로드 끝났어? (const가 없는 건 내부 처리가 있으니까)
			bool isReady();
			///읽기 오류는 없나?
			bool isError() const;

			///쓰기 함수
			void lock(unsigned** address, int* pitch, int mipLevel = 0);
			///부분 쓰기 함수
			void lock(
				unsigned** address,
				int* pitch,
				int x,
				int y,
				int width,
				int height,
				int mipLevel = 0);
			void unlock(unsigned** address, int mipLevel = 0);
			//정보 획득
			int width() const;
			int height() const;
			///두 칸으로 고쳐지기 전의 원래 길이
			int originalWidth() const;
			///두 칸으로 고쳐지기 전의 원래 높이
			int originalHeight() const;
			int mipmapNumber() const;
			/// 파일에 쓰기 시작하다.성공 실패는 체크하지 않는다.디버그용.
			void write(const char* filename);

			//이하 사용자는 의식하지 않는 함수군
			Texture();
			Texture(const Texture&);
			~Texture();
			operator void* () const;
			Texture& operator=(const Texture&);
			void release();
			bool operator==(const Texture&) const;
			bool operator!=(const Texture&) const;
			bool operator<(const Texture&) const;

			class Impl;
			Texture(const Impl*);
		private:
			friend class Manager;
			Impl* mImpl;
		};

	} //namespace Graphics
} //namespace GameLib

#endif
