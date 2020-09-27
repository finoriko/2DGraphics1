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
				//전부 Document가 실체를 가지고 있으므로 여기서는 아무것도 하지 않는다.
				mChildren = 0;
				mAttributes = 0;
				mName = 0;
				mDocument = 0;
			}
			void build(Tag* tag, const char** p, const char* e) {
				//이는 실질 컨스트럭터이므로 두 번째 호출은 부정
				ASSERT(mAttributeNumber == 0 && mChildNumber == 0);
				//이름 이식

				mName = mDocument->allocateString(*tag->name());

				// 어트리뷰트 이식
				mAttributeNumber = tag->attributeNumber();
				mAttributes = mDocument->allocateAttribute(mAttributeNumber);
				tag->giveAttributes(mAttributes);

				//빈 요소 태그 라면 자식이 없기 때문에 여기에 빠진다.
				if (tag->type() == Tag::TYPE_BEGIN_END) {
					return;
				}

				// 일시자 엘리먼트 리스트
				Tank< Element::Impl > children;
				// 태그 찾기
				while (*p < e) {
					if (**p == '<') { //발견
						++(*p); //< 다음으로 이동
						Tag tmpTag(p, e, mDocument); //태그 해석을 통째로 던짐
						Tag::Type type = tmpTag.type();
						if (type == Tag::TYPE_END) { //종료 태그면 끝.빠지다.
							break; //끝내기
						}
						else if (type == Tag::TYPE_COMMENT) {
							; //아무것도 안한다
						}
						else { //시작, 빈 요소 모두 이 처리 필요
							Element::Impl* tmpE = children.add(); //자식에게 추가
							tmpE->setDocument(mDocument);
							tmpE->build(&tmpTag, p, e);
							tmpE = 0;
						}
					}
					else {
						++(*p); //<가 나올 때까지 무시해 버림
					}
				}
				//하위 요소를 본 확보
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
			//단순히 하나씩 불러간다.
			void convertToString(OStringStream* out, int indent) const {
				// 들여쓰기 개수만큼 탭 쓰기
				for (int i = 0; i < indent; ++i) {
					*out << '\t';
				}
				//태그 시작과 엘리먼트 이름
				*out << '<' << mName;
				// 애트리뷰트 글쓰기
				//5개를 경계로 내는 방법을 바꾸다
				int an = mAttributeNumber;
				if (an >= 5) {
					*out << "\r\n";
					//많기 때문에 모두 줄 바꿈
					for (int i = 0; i < an; ++i) {
						//들여 쓰기의 수만큼 탭 쓰기
						for (int j = 0; j < indent; ++j) {
							*out << '\t';
						}
						*out << mAttributes[i].name();
						const char* value = mAttributes[i].value();
						if (value) {
							*out << "=\"" << mAttributes[i].value() << '"';
						}
						if (i != an - 1) {
							*out << "\r\n"; //마지막에는 줄바꿈하지 않는다
						}
					}
				}
				else {
					for (int i = 0; i < an; ++i) {
						*out << ' '; //스페이스
						*out << mAttributes[i].name();
						const char* value = mAttributes[i].value();
						if (value) {
							*out << "=\"" << mAttributes[i].value() << '"';
						}
					}
				}
				if (mChildNumber > 0) { //아이가 있는 경우
					*out << ">\r\n"; //시작 태그 끝
					//아이에게 흘려주다
					for (int i = 0; i < mChildNumber; ++i) {
						// 이름이 세팅되어 있으면.
						if (mChildren[i].name()) {
							mChildren[i].convertToString(out, indent + 1);
						}
					}
					// 들여쓰기 개수만큼 탭 쓰기
					for (int i = 0; i < indent; ++i) {
						*out << '\t';
					}
					//종료태그
					*out << "</" << mName << ">\r\n";
				}
				else {  // 자식이 없다면 빈 요소 태그가 좋음
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
				//자식에게
				out->write(mChildNumber); //childNumber
				for (int i = 0; i < mChildNumber; ++i) {
					if (mChildren[i].name()) { //이름이 있는 것밖에 내놓지 않는다
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
