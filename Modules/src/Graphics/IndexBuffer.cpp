#include <d3d9.h>
#undef min
#undef max

#include "GameLib/GameLib.h"
#include "GameLib/Graphics/IndexBuffer.h"
#include "Graphics/IndexBufferImpl.h"
#include "Graphics/ManagerImpl.h"

namespace GameLib {
	namespace Graphics {

		IndexBuffer IndexBuffer::create(PseudoXml::ConstElement e) {
			IndexBuffer r;
			r.mImpl = NEW Impl(e, gManagerImpl->dxDevice());
			return r;
		}

		IndexBuffer IndexBuffer::create(int indexNumber) {
			IndexBuffer r;
			r.mImpl = NEW Impl(indexNumber, gManagerImpl->dxDevice());
			return r;
		}

		unsigned short* IndexBuffer::lock() {
			ASSERT(mImpl && "Graphics::IndexBuffer : This is empty object.");
			return mImpl->lock();
		}

		void IndexBuffer::unlock(unsigned short** p) {
			//TODO:디버그 시에는 록으로 반환한 포인터를 기억해 두고 여기서 대조해야 한다.
			ASSERT(*p && "Graphics::IndexBuffer : give LOCKED pointer.");
			ASSERT(mImpl && "Graphics::IndexBuffer : This is empty object.");
			mImpl->unlock();
			*p = 0;
		}

		IndexBuffer::IndexBuffer(const Impl*) : mImpl(0) {
		}

		const char* IndexBuffer::name() const {
			return mImpl->mName;
		}

		int IndexBuffer::indexNumber() const {
			return mImpl->mIndexNumber;
		}

#define TYPE IndexBuffer
#include "GameLib/Base/Impl/ReferenceTypeTemplate.h"

	} //namespace Graphics
} //namespace GameLib
