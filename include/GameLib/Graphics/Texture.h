#ifndef INCLUDED_GAMELIB_GRAPHICS_TEXTURE_H
#define INCLUDED_GAMELIB_GRAPHICS_TEXTURE_H

namespace GameLib {
	namespace PseudoXml {
		class ConstElement;
	}
	namespace Graphics {

		class Texture {
		public:
			///������Ʈ���� ����.path�� ������ ���� �̸��� �߰��Ѵ�.
			static Texture create(PseudoXml::ConstElement, const char* path = 0);
			// �� �ؽ�ó
			static Texture create(int width, int height, bool createMipChain);
			///TGA Ȥ�� DDS. Ȯ���ڷ� �Ǻ��Ѵ�.
			static Texture create(const char* filename);
			///TGA Ȥ�� DDS�� ���빰�� �����Ѵ�.ó���� DDS��� �� ������ DDS�� �����ϰ�, �� �̿ܿ��� TGA�� �����Ѵ�.
			static Texture create(const char* fileData, int fileSize);
			///�̸� ��������(��¥xml���� �ε��ϸ� �� �̸�.�ٸ� ���� ����ִ�).
			const char* name() const;

			///�ε� ������? (const�� ���� �� ���� ó���� �����ϱ�)
			bool isReady();
			///�б� ������ ����?
			bool isError() const;

			///���� �Լ�
			void lock(unsigned** address, int* pitch, int mipLevel = 0);
			///�κ� ���� �Լ�
			void lock(
				unsigned** address,
				int* pitch,
				int x,
				int y,
				int width,
				int height,
				int mipLevel = 0);
			void unlock(unsigned** address, int mipLevel = 0);
			//���� ȹ��
			int width() const;
			int height() const;
			///�� ĭ���� �������� ���� ���� ����
			int originalWidth() const;
			///�� ĭ���� �������� ���� ���� ����
			int originalHeight() const;
			int mipmapNumber() const;
			/// ���Ͽ� ���� �����ϴ�.���� ���д� üũ���� �ʴ´�.����׿�.
			void write(const char* filename);

			//���� ����ڴ� �ǽ����� �ʴ� �Լ���
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
