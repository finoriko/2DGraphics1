#include "GameLib/GameLib.h"
#include "PseudoXml/Tag.h"
#include "PseudoXml/AttributeImpl.h"
#include "PseudoXml/DocumentImpl.h"
#include "GameLib/Base/RefString.h"
using namespace std;

namespace GameLib {
	namespace PseudoXml {

		// ���� õ��ǥ.
		/*
		<���� ��ȣ ����Ʈ>
		0:<��.�ʱ� ����
		1: ������Ʈ ���ڿ�
		��Ҹ� ���� ����.��Ʈ����Ʈ ������ ����.
		3:��Ʈ����Ʈ��
		4:=��
		Attribute��("�� ���빰)
		6: �� ��� �±� ��ȣ/��
		��Ʈ����Ʈ��� = ������ ����
		Attribute��('�� ���빰)
		��Ʈ����Ʈ��(������ ���� ��)
		�ڸ�Ʈ ...>�� �ڸ�Ʈ. ����� <!-->������,!�� ���� �ܰ迡�� ������ �ƴϰ� ����.(��� ���� ������...ELEMENT�����͵� �־�)
		�ڸ�Ʈ��-�߰�
		�ڸ�Ʈ��--�߰�
		�ڸ�Ʈ�� < ? ... ?>�� �ڸ�Ʈ
		�ڸ�Ʈ��? �߰�
		E: ����
		
		<���� ���� ����Ʈ>
		��ȣ:
		c = [a-zA-Z0-9_] �� ���� ����
		s = �ٸ� ���ǿ� �ɸ��� �ʴ� ������ ����
		
		0,/,0 ���� �±� �÷��� �����
		0,!, 10 �ڸ�Ʈ���ݾ�
		0, ? 13 ������ݾ�
		0,c,1 ������Ʈ ���ڿ��� ù �� ����
		0,s,0 �ӹ�����
		1,c,1 ������Ʈ ���ڿ��� �߰�
		1,>,E
		1,s,2 ������Ʈ�� Ȯ��
		2,>,E
		2,/,6 ����� �±�/�߰�
		2,c,3 ��Ʈ����Ʈ�� ������ �� ����
		2,s,2 �о� �ѱ��
		3,=,4 = �߰�.��Ʈ����Ʈ��Ȯ��
		3,c,3 ��Ʈ����Ʈ�� �߰�
		3,s,7 ��������.��Ʈ����Ʈ��Ȯ��
		4,5 ���� �����̼� �߰�.��Ʈ����Ʈ������ ����
		4,/,8 �̱� �����̼� �߰�.��Ʈ����Ʈ������ ����
		4, c, 9 ��Ʈ����Ʈ ������ ����
		4,s,4 �ǳʶٴ�
		5,",2��Ʈ����Ʈ��Ȯ��.��Ʈ����Ʈ�� ����Ʈ�� �߰�.
		5,5,5 Attribute �� ���ڿ��� �߰�
		6, >, E����ҷμ� ������.
		6,s,6 �о� �ѱ��
		7,=,4 = �߰�
		7,s,7 �о� �ѱ��
		8,',2��Ʈ����Ʈ��Ȯ��.
		8,s,8 ��Ʈ����Ʈ ���� �߰�
		9,/, 6�� ��� �±� �߰�.
		9,>,E
		9,c,9 ��Ʈ����Ʈ ���� �߰�
		9,s,2 ��Ʈ����Ʈ�� Ȯ��
		10,-,11
		��, s, ��
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
			// ������Ʈ �̸�, ��Ʈ����Ʈ �̸� �� ���� �ӽ� �ݳ����
			const char* eName = 0;
			int eNameSize = 0;
			const char* aName = 0;
			int aNameSize = 0;
			const char* aValue = 0;
			int aValueSize = 0;

			int m = 0; // ���. mode ���� �ɷ� �ϴ� �� �������� ���Ƚ���� �ɻ�ġ �����ϱ� 1���ڷ� �ߴ�
			bool end = false; // ������ ���������� �÷���
			while (*p < e) {
				bool wide = false; //�Ϻ����ΰ���?
				char c = **p;
				switch (m) {
				case 0: //�ʱ����
					if (c == '/') {
						mType = TYPE_END;
					}
					else if (c == '!') { //�ڸ�Ʈ
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
						; //�ƹ��͵� ���Ѵ�
					}
					break;
				case 1: //������Ʈ��
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
						m = 2; //������Ʈ���� ����
					}
					break;
				case 2: //������Ʈ�̸��� ����
					if (c == '>') {
						end = true;
					}
					else if (c == '/') {
						mType = TYPE_BEGIN_END; //�����±��Դϴ�
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
						; //�ƹ��͵� ���Ѵ�
					}
					break;
				case 3: //��Ʈ����Ʈ �̸�
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
						m = 7; //��������
					}
					break;
				case 4: //=�� ��
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
						; //�ƹ��͵� ���Ѵ�
					}
					break;
				case 5: //��Ʈ����Ʈ ��
					if (c == '"') {
						m = 2; //������Ʈ�̸����ǰ���
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
					}
					else {
						++aValueSize;
						isNormalChar(c, &wide); //���̵������� �˰�ʹ�
						if (wide && ((*p + 1) < e)) {
							++aValueSize;
						}
					}
					break;
				case 6: //�������±���/������
					switch (c) {
					case '>': end = true; break;
					default: break; //��� �о��ֱ�
					}
					break;
				case 7: //��Ʈ����Ʈ �̸� ���� ����
					if (c == '=') {
						m = 4;
					}
					else if (c == '>') {
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
						end = true;
					}
					else if (isNormalChar(c, &wide)) { //���� ���� ��Ʈ����Ʈ
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
						//���� ��Ʈ����Ʈ�� �ְڽ��ϴ�.
						aName = *p;
						aNameSize = 1;
						if (wide && ((*p + 1) < e)) {
							++aNameSize;
						}
						m = 3;
					}
					else {
						; //�ƹ��͵� ����
					}
					break;
				case 8: // ��Ʈ����Ʈ ��
					if (c == '\'') {
						m = 2; //������Ʈ �̸� ���� ����
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
					}
					else {
						++aValueSize;
						isNormalChar(c, &wide); //���̵� ������ �˰�ʹ�.
						if (wide && ((*p + 1) < e)) {
							++aValueSize;
						}
					}
					break;
				case 9: //��Ʈ����Ʈ ��
					if (c == '/') {
						mType = TYPE_BEGIN_END; //�� ��� �±��Դϴ�
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
						m = 2; //������Ʈ �̸� ���� ����
						addAttribute(aName, aNameSize, aValue, aValueSize);
						aNameSize = aValueSize = 0;
					}
				case 10: //�ڸ�Ʈ
					switch (c) {
					case '-': m = 11; break;
					default: break; // ��� �о��ֱ�
					}
					break;
				case 11: //�ڸ�Ʈ
					switch (c) {
					case '-': m = 12; break;
					default: m = 10; break; //�ڸ�Ʈ���
					}
					break;
				case 12: //�ڸ�Ʈ
					switch (c) {
					case '>': end = true; break;
					default: m = 10; break; //�ڸ�Ʈ���
					}
					break;
				case 13: //?�ڸ�Ʈ
					switch (c) {
					case '?': m = 14; break;
					default: break; // ��� �о��ֱ�
					}
					break;
				case 14: //?�ڸ�Ʈ?
					switch (c) {
					case '>': end = true; break;
					default: m = 13; break; //�ڸ�Ʈ���
					}
					break;
				}
				*p += (wide) ? 2 : 1; //����
				if (end) { // ���Դϴ�. �����ϰ� ���� ������ �Ϳ� ��������.���ٰ� �ݴ�� ���� �����Ѵ�
					break;
				}
			}
			mName.set(eName, eNameSize);
		}

	} //namespace PseudoXml
} //namespace GameLib
