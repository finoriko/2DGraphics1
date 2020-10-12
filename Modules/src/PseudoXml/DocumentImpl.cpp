#include "PseudoXml/ElementImpl.h"
#include "PseudoXml/AttributeImpl.h"

#include "GameLib/PseudoXml/Document.h"

namespace GameLib 
{
	namespace PseudoXml 
	{
		Document::Impl::Impl() :
			mStringPool(1024), //일단 1KB씩
			mElementPool(16), //열여섯 개씩
			mAttributePool(16), //열여섯 개씩
			mRoot(0) {
			mRoot = allocateElement(1);
			mRoot->setDocument(this);
		}

		Document::Impl::~Impl() 
		{
		}

		bool Document::Impl::isReady() {
			bool r = false;
			if (mRoot->name()) { //이름이 NULL이 아니라면 build 완료.
				r = true;
			}
			else if (mFile) { //로드중
				if (mFile.isFinished()) { //로드가 끝났다
					build(mFile.data(), mFile.size());
					mFile.release();
					r = true;
				}
			}
			else {
				HALT("IT MUST BE BUG!"); //루트 요소도 없이
			}
			return r;
		}

		void Document::Impl::build(const char* data, int size) {
			if (RefString(data, 4) == "PXML") { //이진이다!
				IBinaryStream is(data, size);
				is.setPosition(4);
				//풀에 문자열을 처넣다
				int stringBufferOffset = is.readInt();
				int stringBufferSize = size - stringBufferOffset;
				is.setPosition(stringBufferOffset);
				char* stringBuffer = mStringPool.allocate(stringBufferSize);
				is.read(stringBuffer, stringBufferSize);
				is.setPosition(8);
				mRoot->setName("");
				mRoot->setChildNumber(is.readInt());
				for (int i = 0; i < mRoot->childNumber(); ++i) {
					mRoot->child(i)->build(&is, stringBuffer);
				}
			}
			else { //텍스트이다
				const char* begin = data;
				const char* end = begin + size;
				Tag tag("<ROOT>");
				mRoot->build(&tag, &begin, end);
			}
		}

		//단순하게 하나씩 불러간다.
		void Document::Impl::convertToString(string* out) const {
			OStringStream oss;
			int childN = mRoot->childNumber();
			for (int i = 0; i < childN; ++i) {
				const Element::Impl* e = mRoot->child(i);
				if (e->name()) { //이름이 있는 것밖에 내놓지 않는다
					e->convertToString(&oss, 0);
				}
			}
			Array< char > t;
			oss.get(&t);
			*out = &t[0];
		}

		//단순하게 하나씩 불러간다.
		void Document::Impl::convertToBinary(Array< char >* out) const {
			int childN = mRoot->childNumber();
			OStringStream binaryOut;
			OStringStream stringBuffer;
			map< RefString, int > stringMap;
			binaryOut.write("PXML", 4); //매직
			binaryOut.write(0); //스트링 버퍼 오프셋의 더미
			binaryOut.write(childN); //요소수
			for (int i = 0; i < childN; ++i) {
				const Element::Impl* e = mRoot->child(i);
				if (e->name()) { //이름이 있는 것밖에 내놓지 않는다
					e->convertToBinary(&binaryOut, &stringBuffer, &stringMap);
				}
			}
			int stringBufferOffset = binaryOut.size();
			//스트링 버퍼 머지
			stringBuffer.get(out);
			binaryOut.write(&(*out)[0], out->size());
			out->clear();

			binaryOut.get(out);
			//스트링 버퍼 오프셋 진품을 넣다
			const char* p = reinterpret_cast<const char*>(&stringBufferOffset);
			for (int i = 0; i < 4; ++i) {
				(*out)[4 + i] = p[i];
			}
		}

		Element::Impl* Document::Impl::root() {
			return mRoot;
		}

		Element::Impl* Document::Impl::allocateElement(int n) {
			return mElementPool.allocate(n);
		}

		Attribute::Impl* Document::Impl::allocateAttribute(int n) {
			return mAttributePool.allocate(n);
		}
	}
}