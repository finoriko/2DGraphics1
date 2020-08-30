#ifndef INCLUDED_GAMELIB_GRAPHICS_INDEXBUFFER_H
#define INCLUDED_GAMELIB_GRAPHICS_INDEXBUFFER_H

namespace GameLib {
	namespace PseudoXml {
		class ConstElement;
	}
	namespace Graphics {

		class IndexBuffer {
		public:
			static IndexBuffer create(PseudoXml::ConstElement);
			static IndexBuffer create(int indexNumber);
			unsigned short* lock();
			void unlock(unsigned short**);
			const char* name() const;
			int indexNumber() const;

			//이하 사용자는 의식하지 않는 함수군
			IndexBuffer();
			~IndexBuffer();
			IndexBuffer(const IndexBuffer&);
			operator void* () const;
			IndexBuffer& operator=(const IndexBuffer&);
			void release();
			bool operator==(const IndexBuffer&) const;
			bool operator!=(const IndexBuffer&) const;
			class Impl;
			IndexBuffer(const Impl*);
		private:
			friend class Manager;
			Impl* mImpl;
		};

	} //namespace Graphics
} //namespace GameLib

#endif