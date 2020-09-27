#ifndef INCLUDED_GAMELIB_PSEUDOXML_ElEMENTIMPL_H
#define INCLUDED_GAMELIB_PSEUDOXML_ElEMENTIMPL_HH

#include "GameLib/Base/Vector.h"
#include "GameLib/Base/OStringStream.h"
#include "GameLib/Base/IBinaryStream.h"
#include "PseudoXml/DocumentImpl.h"
#include "PseudoXml/Tag.h"
#include <map>
using namespace std;

namespace GameLib {
	namespace PseudoXml {

		class Element::Impl {
		public:
			Impl() :
				mChildren(0),
				mChildNumber(0),
				mAttributes(0),
				mAttributeNumber(0),
				mName(0),
				mDocument(0) {
			}
			~Impl() {
				//���� Document�� ��ü�� ������ �����Ƿ� ���⼭�� �ƹ��͵� ���� �ʴ´�.
				mChildren = 0;
				mAttributes = 0;
				mName = 0;
				mDocument = 0;
			}
			void build(Tag* tag, const char** p, const char* e) {
				//�̴� ���� ����Ʈ�����̹Ƿ� �� ��° ȣ���� ����
				ASSERT(mAttributeNumber == 0 && mChildNumber == 0);
				//�̸� �̽�

				mName = mDocument->allocateString(*tag->name());

				// ��Ʈ����Ʈ �̽�
				mAttributeNumber = tag->attributeNumber();
				mAttributes = mDocument->allocateAttribute(mAttributeNumber);
				tag->giveAttributes(mAttributes);

				//�� ��� �±� ��� �ڽ��� ���� ������ ���⿡ ������.
				if (tag->type() == Tag::TYPE_BEGIN_END) {
					return;
				}

				// �Ͻ��� ������Ʈ ����Ʈ
				Tank< Element::Impl > children;
				// �±� ã��
				while (*p < e) {
					if (**p == '<') { //�߰�
						++(*p); //< �������� �̵�
						Tag tmpTag(p, e, mDocument); //�±� �ؼ��� ��°�� ����
						Tag::Type type = tmpTag.type();
						if (type == Tag::TYPE_END) { //���� �±׸� ��.������.
							break; //������
						}
						else if (type == Tag::TYPE_COMMENT) {
							; //�ƹ��͵� ���Ѵ�
						}
						else { //����, �� ��� ��� �� ó�� �ʿ�
							Element::Impl* tmpE = children.add(); //�ڽĿ��� �߰�
							tmpE->setDocument(mDocument);
							tmpE->build(&tmpTag, p, e);
							tmpE = 0;
						}
					}
					else {
						++(*p); //<�� ���� ������ ������ ����
					}
				}
				//���� ��Ҹ� �� Ȯ��
				mChildNumber = children.size();
				mChildren = mDocument->allocateElement(mChildNumber);
				children.copyTo(mChildren);
			}
			void build(IBinaryStream* s, const char* stringBuffer) {
				mName = stringBuffer + s->readInt();
				mAttributeNumber = s->readInt();
				mAttributes = mDocument->allocateAttribute(mAttributeNumber);
				for (int i = 0; i < mAttributeNumber; ++i) {
					const char* name = stringBuffer + s->readInt();
					const char* value = stringBuffer + s->readInt();
					mAttributes[i].setDocument(mDocument);
					mAttributes[i].setNameReference(name);
					if (*value != '\0') {
						mAttributes[i].setValueReference(value);
					}
				}
				mChildNumber = s->readInt();
				mChildren = mDocument->allocateElement(mChildNumber);
				for (int i = 0; i < mChildNumber; ++i) {
					mChildren[i].setDocument(mDocument);
					mChildren[i].build(s, stringBuffer);
				}
			}
			//�ܼ��� �ϳ��� �ҷ�����.
			void convertToString(OStringStream* out, int indent) const {
				// �鿩���� ������ŭ �� ����
				for (int i = 0; i < indent; ++i) {
					*out << '\t';
				}
				//�±� ���۰� ������Ʈ �̸�
				*out << '<' << mName;
				// ��Ʈ����Ʈ �۾���
				//5���� ���� ���� ����� �ٲٴ�
				int an = mAttributeNumber;
				if (an >= 5) {
					*out << "\r\n";
					//���� ������ ��� �� �ٲ�
					for (int i = 0; i < an; ++i) {
						//�鿩 ������ ����ŭ �� ����
						for (int j = 0; j < indent; ++j) {
							*out << '\t';
						}
						*out << mAttributes[i].name();
						const char* value = mAttributes[i].value();
						if (value) {
							*out << "=\"" << mAttributes[i].value() << '"';
						}
						if (i != an - 1) {
							*out << "\r\n"; //���������� �ٹٲ����� �ʴ´�
						}
					}
				}
				else {
					for (int i = 0; i < an; ++i) {
						*out << ' '; //�����̽�
						*out << mAttributes[i].name();
						const char* value = mAttributes[i].value();
						if (value) {
							*out << "=\"" << mAttributes[i].value() << '"';
						}
					}
				}
				if (mChildNumber > 0) { //���̰� �ִ� ���
					*out << ">\r\n"; //���� �±� ��
					//���̿��� ����ִ�
					for (int i = 0; i < mChildNumber; ++i) {
						// �̸��� ���õǾ� ������.
						if (mChildren[i].name()) {
							mChildren[i].convertToString(out, indent + 1);
						}
					}
					// �鿩���� ������ŭ �� ����
					for (int i = 0; i < indent; ++i) {
						*out << '\t';
					}
					//�����±�
					*out << "</" << mName << ">\r\n";
				}
				else {  // �ڽ��� ���ٸ� �� ��� �±װ� ����
					*out << "/>\r\n";
				}
			}
			void convertToBinary(
				OStringStream* out,
				OStringStream* stringBuffer,
				map< RefString, int >* stringMap) const {
				typedef map< RefString, int > Map;
				RefString name(mName);
				Map::iterator it = stringMap->find(name);
				int pos = 0;
				if (it == stringMap->end()) {
					pos = stringBuffer->size();
					*stringBuffer << name;
					stringBuffer->write("\0", 1);
					stringMap->insert(make_pair(name, pos));
				}
				else {
					pos = it->second;
				}
				out->write(pos); //nameOffset
				out->write(mAttributeNumber); //attributeNumber
				for (int i = 0; i < mAttributeNumber; ++i) {
					const Attribute::Impl& a = mAttributes[i];
					RefString aName(a.name());
					RefString aValue;
					if (a.value()) {
						aValue = a.value();
					}
					//name
					it = stringMap->find(aName);
					pos = 0;
					if (it == stringMap->end()) {
						pos = stringBuffer->size();
						*stringBuffer << aName;
						stringBuffer->write("\0", 1);
						stringMap->insert(make_pair(aName, pos));
					}
					else {
						pos = it->second;
					}
					out->write(pos); //Attribute::nameOffset
					//value
					it = stringMap->find(aValue);
					pos = 0;
					if (it == stringMap->end()) {
						pos = stringBuffer->size();
						*stringBuffer << aValue;
						stringBuffer->write("\0", 1);
						stringMap->insert(make_pair(aValue, pos));
					}
					else {
						pos = it->second;
					}
					out->write(pos); //Attribute::valueOffset
				}
				//�ڽĿ���
				out->write(mChildNumber); //childNumber
				for (int i = 0; i < mChildNumber; ++i) {
					if (mChildren[i].name()) { //�̸��� �ִ� �͹ۿ� ������ �ʴ´�
						mChildren[i].convertToBinary(out, stringBuffer, stringMap);
					}
				}
			}
			int attributeNumber() const {
				return mAttributeNumber;
			}
			void setAttributeNumber(int n) {
				ASSERT(mAttributeNumber == 0);
				mAttributeNumber = n;
				mAttributes = mDocument->allocateAttribute(n);
				for (int i = 0; i < mAttributeNumber; ++i) {
					mAttributes[i].setDocument(mDocument);
				}
			}
			Attribute::Impl* attribute(int i) {
				ASSERT(i >= 0 && i < mAttributeNumber);
				return &mAttributes[i];
			}
			const Attribute::Impl* attribute(int i) const {
				return &mAttributes[i];
			}
			int childNumber() const {
				return mChildNumber;
			}
			void setChildNumber(int n) {
				ASSERT(mChildNumber == 0);
				mChildNumber = n;
				mChildren = mDocument->allocateElement(n);
				for (int i = 0; i < mChildNumber; ++i) {
					mChildren[i].setDocument(mDocument);
				}
			}
			Element::Impl* child(int i) {
				return &mChildren[i];
			}
			const Element::Impl* child(int i) const {
				return &mChildren[i];
			}
			const char* name() const {
				return mName;
			}
			void setName(const char* name) {
				mName = mDocument->allocateString(name);
			}
			void setNameReference(const char* name) {
				mName = name;
			}
			void setDocument(Document::Impl* d) {
				mDocument = d;
			}
			Document::Impl* document() {
				return mDocument;
			}
		private:
			Element::Impl* mChildren;
			int mChildNumber;
			Attribute::Impl* mAttributes;
			int mAttributeNumber;
			const char* mName;
			Document::Impl* mDocument;
		};

	} //namespace PseudoXml
} //namespace GameLib

#endif
