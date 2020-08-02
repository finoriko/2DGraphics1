#ifndef INCLUDED_GAMELIB_SCENE_FONT_H
#define INCLUDED_GAMELIB_SCENE_FONT_H

namespace GameLib {

	namespace Graphics {
		class Texture;
	}
	namespace Math {
		class Vector2;
	}
	using namespace Math;

	namespace Scene {

		///��ƮŬ����
		class Font {
		public:
			///��Ʈ�� �۲� �ϳ��ϳ��� �ʿ��� ������
			struct BitmapChar {
				unsigned shortmCode; // �����ڵ�(SHIFT_JIS)
				unsigned shortmX; // ���� ��
				unsigned short mY; // ���
				unsigned charmTexturePage; // �ؽ�ó ������
				unsigned charmWidth; // �� ����(�ȼ�)
				unsigned charmHeight; // ���� ����(�ȼ�)
			};
			////// �ؽ�ó�� ����� ��Ʈ�� ��Ʈ�� ����.������ ���̴� �ִ�ġ�� ä��.
			static Font create(
				const char** textureFileNames, //�ؽ�ó ���� �̸� �迭
				int textureNumber,
				BitmapChar* BitmapChars,
				int bitmapCharNumber);
			///�ؽ�ó�� ����� ��Ʈ�� ��Ʈ�� ����.������ ���̴� �ִ�ġ�� ä��.
			static Font create(
				const char** textureFileData, //�ؽ�ó DDS �̹��� �迭
				const int* textureFileSizes, //�ؽ�ó ������ ũ�� �迭
				int textureNumber,
				BitmapChar* bitmapChars,
				int CharNumber);
			///OS���� �������� ��Ʈ �����͸� �����´�.������ ����, ���� �ؽ�ó �� ���� ����.
			static Font create(
				const char* osFontName,
				int charHeightHint, //�̰� �����̾�. ������ �� ���� getCharHeight�� �����϶�.
				bool bold = false,
				bool italic = false,
				bool proportional = false,
				bool antialias = false,
				int internalTextureWidth = 512,
				int internalTextureHeight = 512);
			///���� ���� ��������.�߰ߵ��� ������ false�� ��ȯ�Ѵ�.
			bool getChar(
				Graphics::Texture* textureOut,
				Vector2* uvTopLeftOut,
				Vector2* uvBottomRightOut,
				int code);
			///������ �ִ� ����
			int charHeight() const;
			///�ε� ������?
			bool isReady();

			//���� ����ڴ� �ǽ����� �ʾƵ� �ȴ�
			Font();
			~Font();
			Font(const Font&);
			operator void* () const;
			Font& operator=(const Font&);
			void release();
			bool operator==(const Font&) const;
			bool operator!=(const Font&) const;

			class Impl;
		private:
			explicit Font(Impl*);
			Impl* mImpl;
		};

	} //namespace Scene
} //namespace GameLib

#endif
