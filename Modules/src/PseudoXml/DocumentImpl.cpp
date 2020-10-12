#include "PseudoXml/ElementImpl.h"
#include "PseudoXml/AttributeImpl.h"

#include "GameLib/PseudoXml/Document.h"

namespace GameLib 
{
	namespace PseudoXml 
	{
		Document::Impl::Impl() :
			mStringPool(1024), //�ϴ� 1KB��
			mElementPool(16), //������ ����
			mAttributePool(16), //������ ����
			mRoot(0) {
			mRoot = allocateElement(1);
			mRoot->setDocument(this);
		}

		Document::Impl::~Impl() 
		{
		}

		bool Document::Impl::isReady() {
			bool r = false;
			if (mRoot->name()) { //�̸��� NULL�� �ƴ϶�� build �Ϸ�.
				r = true;
			}
			else if (mFile) { //�ε���
				if (mFile.isFinished()) { //�ε尡 ������
					build(mFile.data(), mFile.size());
					mFile.release();
					r = true;
				}
			}
			else {
				HALT("IT MUST BE BUG!"); //��Ʈ ��ҵ� ����
			}
			return r;
		}

		void Document::Impl::build(const char* data, int size) {
			if (RefString(data, 4) == "PXML") { //�����̴�!
				IBinaryStream is(data, size);
				is.setPosition(4);
				//Ǯ�� ���ڿ��� ó�ִ�
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
			else { //�ؽ�Ʈ�̴�
				const char* begin = data;
				const char* end = begin + size;
				Tag tag("<ROOT>");
				mRoot->build(&tag, &begin, end);
			}
		}

		//�ܼ��ϰ� �ϳ��� �ҷ�����.
		void Document::Impl::convertToString(string* out) const {
			OStringStream oss;
			int childN = mRoot->childNumber();
			for (int i = 0; i < childN; ++i) {
				const Element::Impl* e = mRoot->child(i);
				if (e->name()) { //�̸��� �ִ� �͹ۿ� ������ �ʴ´�
					e->convertToString(&oss, 0);
				}
			}
			Array< char > t;
			oss.get(&t);
			*out = &t[0];
		}

		//�ܼ��ϰ� �ϳ��� �ҷ�����.
		void Document::Impl::convertToBinary(Array< char >* out) const {
			int childN = mRoot->childNumber();
			OStringStream binaryOut;
			OStringStream stringBuffer;
			map< RefString, int > stringMap;
			binaryOut.write("PXML", 4); //����
			binaryOut.write(0); //��Ʈ�� ���� �������� ����
			binaryOut.write(childN); //��Ҽ�
			for (int i = 0; i < childN; ++i) {
				const Element::Impl* e = mRoot->child(i);
				if (e->name()) { //�̸��� �ִ� �͹ۿ� ������ �ʴ´�
					e->convertToBinary(&binaryOut, &stringBuffer, &stringMap);
				}
			}
			int stringBufferOffset = binaryOut.size();
			//��Ʈ�� ���� ����
			stringBuffer.get(out);
			binaryOut.write(&(*out)[0], out->size());
			out->clear();

			binaryOut.get(out);
			//��Ʈ�� ���� ������ ��ǰ�� �ִ�
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