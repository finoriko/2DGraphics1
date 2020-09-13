#include "GameLib/GameLib.h"
#include "PseudoXml/Tag.h"
#include "PseudoXml/AttributeImpl.h"
#include "PseudoXml/DocumentImpl.h"
#include "GameLib/Base/RefString.h"
using namespace std;

namespace GameLib {
	namespace PseudoXml {

		// 상태 천이표.
		/*
		<상태 번호 리스트>
		0:<위.초기 상태
		1: 엘리먼트 명문자열
		요소명 뒤의 공백.애트리뷰트 사이의 공백.
		3:아트리뷰트명
		4:=위
		Attribute값("의 내용물)
		6: 빈 요소 태그 기호/위
		아트리뷰트명과 = 사이의 공백
		Attribute값('의 내용물)
		애트리뷰트값(싸이지 않은 놈)
		코멘트 ...>가 코멘트. 사실은 <!-->이지만,!가 나온 단계에서 보통이 아니고 무시.(사실 맛은 없지만...ELEMENT같은것도 있어)
		코멘트중-발견
		코멘트중--발견
		코멘트중 < ? ... ?>가 코멘트
		코멘트중? 발견
		E: 종료
		
		<상태 전이 리스트>
		기호:
		c = [a-zA-Z0-9_] 즉 보통 문자
		s = 다른 조건에 걸리지 않는 나머지 문자
		
		0,/,0 종료 태그 플래그 세우기
		0,!, 10 코멘트가잖아
		0, ? 13 댓글이잖아
		0,c,1 엘리먼트 명문자열의 첫 한 글자
		0,s,0 머무르다
		1,c,1 엘리먼트 명문자열에 추가
		1,>,E
		1,s,2 엘리먼트명 확정
		2,>,E
		2,/,6 공요소 태그/발견
		2,c,3 애트리뷰트명 최초의 한 글자
		2,s,2 읽어 넘기다
		3,=,4 = 발견.애트리뷰트명확정
		3,c,3 애트리뷰트명에 추가
		3,s,7 공백으로.애트리뷰트명확정
		4,5 더블 쿼테이션 발견.애트리뷰트값으로 이행
		4,/,8 싱글 쿼테이션 발견.애트리뷰트값으로 이행
		4, c, 9 어트리뷰트 값으로 이행
		4,s,4 건너뛰다
		5,",2아트리뷰트값확정.애트리뷰트를 리스트에 추가.
		5,5,5 Attribute 값 문자열에 추가
		6, >, E공요소로서 끝나다.
		6,s,6 읽어 넘기다
		7,=,4 = 발견
		7,s,7 읽어 넘기다
		8,',2아트리뷰트값확정.
		8,s,8 애트리뷰트 값에 추가
		9,/, 6공 요소 태그 발견.
		9,>,E
		9,c,9 애트리뷰트 값에 추가
		9,s,2 아트리뷰트값 확정
		10,-,11
		십, s, 십
		11,-,12
		11,s,10
		12, >, E
		12,s,10
		13,?,14
		13,s,13
		14, >, E
		14,s,13
		*/
		Tag::Tag(const char** p, const char* e, Document::Impl* document) :
			mType(TYPE_BEGIN),
			mDocument(document) {
			// 엘리먼트 이름, 어트리뷰트 이름 및 값의 임시 격납장소
			const char* eName = 0;
			int eNameSize = 0;
			const char* aName = 0;
			int aNameSize = 0;
			const char* aValue = 0;
			int aValueSize = 0;

			int m = 0; // 모드. mode 같은 걸로 하는 게 좋겠지만 사용횟수가 심상치 않으니까 1문자로 했다
			bool end = false; // 루프를 빠져나가는 플래그
			while (*p < e) {
				bool wide = false; //일본어인가요?
				char c = **p;
				switch (m) {
				case 0: //초기상태
					if (c == '/') {
						mType = TYPE_END;
					}
					else if (c == '!') { //코멘트
						mType = TYPE_COMMENT;
						m = 10;
					}
					else if (c == '?') {
						mType = TYPE_COMMENT;
						m = 13;
					}
					else if (isNormalChar(c, &wide)) {
						eName = *p;
						eNameSize = 1;
						if (wide && ((*p + 1) < e)) {
							++eNameSize;
						}
						m = 1;
					}
					else {
						; //아무것도 안한다
					}
					break;
				case 1: //엘리먼트명
					if (c == '>') {
						end = true;
					}
					else if (isNormalChar(c, &wide)) {
						++eNameSize;
						if (wide && ((*p + 1) < e)) {
							++eNameSize;
						}
					}
					else {
						m = 2; //엘리먼트명을 빠짐
					}
					break;
				case 2: //엘리먼트이름뒤 공백
					if (c == '>') {
						end = true;
					}
					else if (c == '/') {
						mType = TYPE_BEGIN_END; //빈요소태그입니다
						m = 6;
					}
					else if (isNormalChar(c, &wide)) {
						aName = *p;
						aNameSize = 1;
						if (wide && ((*p + 1) < e)) {
							++aNameSize;
						}
						m = 3;
					}
					else {
						; //아무것도 안한다
					}
					break;
				case 3: //어트리뷰트 이름
					if (c == '=') {
						m = 4;
					}
					else if (isNormalChar(c, &wide)) {
						++aNameSize;
						if (wide && ((*p + 1) < e)) {
							++aNameSize;
						}
					}
					else {
						m = 7; //공백으로
					}
					break;
				case 4: //=는 위
					if (c == '"') {
						aValue = *p + 1;
						m = 5;
					}
					else if (c == '\'') {
						aValue = *p + 1;
						m = 8;
					}
					else if (isNormalChar(c, &wide)) {
						aValue = *p;
						aValueSize = 1;
						if (wide && ((*p + 1) < e)) {
							++aValueSize;
						}
						m = 9;
					}
					else {
						; //아무것도 안한다
					}
					break;
				case 5: //어트리뷰트 값
					if (c == '"') {
						m = 2; //엘리먼트이름뒤의공백
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
					}
					else {
						++aValueSize;
						isNormalChar(c, &wide); //와이드인지만 알고싶다
						if (wide && ((*p + 1) < e)) {
							++aValueSize;
						}
					}
					break;
				case 6: //공백요소태그의/출현후
					switch (c) {
					case '>': end = true; break;
					default: break; //계속 읽어주기
					}
					break;
				case 7: //애트리뷰트 이름 뒤의 공백
					if (c == '=') {
						m = 4;
					}
					else if (c == '>') {
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
						end = true;
					}
					else if (isNormalChar(c, &wide)) { //값이 없는 애트리뷰트
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
						//다음 애트리뷰트에 넣겠습니다.
						aName = *p;
						aNameSize = 1;
						if (wide && ((*p + 1) < e)) {
							++aNameSize;
						}
						m = 3;
					}
					else {
						; //아무것도 안함
					}
					break;
				case 8: // 애트리뷰트 값
					if (c == '\'') {
						m = 2; //엘리먼트 이름 뒤의 공백
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
					}
					else {
						++aValueSize;
						isNormalChar(c, &wide); //와이드 인지만 알고싶다.
						if (wide && ((*p + 1) < e)) {
							++aValueSize;
						}
					}
					break;
				case 9: //애트리뷰트 값
					if (c == '/') {
						mType = TYPE_BEGIN_END; //빈 요소 태그입니다
						m = 6;
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
					}
					else if (c == '>') {
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
						end = true;
					}
					else if (isNormalChar(c, &wide)) {
						++aValueSize;
						if (wide && ((*p + 1) < e)) {
							++aValueSize;
						}
					}
					else {
						m = 2; //엘리먼트 이름 뒤의 공백
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
					}
				case 10: //코멘트
					switch (c) {
					case '-': m = 11; break;
					default: break; // 계속 읽어주기
					}
					break;
				case 11: //코멘트
					switch (c) {
					case '-': m = 12; break;
					default: m = 10; break; //코멘트계속
					}
					break;
				case 12: //코멘트
					switch (c) {
					case '>': end = true; break;
					default: m = 10; break; //코멘트계속
					}
					break;
				case 13: //?코멘트
					switch (c) {
					case '?': m = 14; break;
					default: break; // 계속 읽어주기
					}
					break;
				case 14: //?코멘트?
					switch (c) {
					case '>': end = true; break;
					default: m = 13; break; //코멘트계속
					}
					break;
				}
				*p += (wide) ? 2 : 1; //진행
				if (end) { // 끝입니다. 진행하고 나서 끝나는 것에 주의하자.윗줄과 반대로 가면 버그한다
					break;
				}
			}
			mName.set(eName, eNameSize);
		}

	} //namespace PseudoXml
} //namespace GameLib
