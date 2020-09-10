#ifndef INCLUDED_GAMELIB_PSEUDOXML_DOCUMENT_H
#define INCLUDED_GAMELIB_PSEUDOXML_DOCUMENT_H

#include <string>

namespace GameLib {
	template< class T > class Array;
	namespace PseudoXml {
		using namespace std;

		class Element;
		class ConstElement;

		///���� �׼��� ����� ���� ��¥ xml �ٿ� Ŭ����
		class Document {
		public:
			///��������� ����ִ°� ����
			static Document create();
			///���Ͽ��� ����Ʈ��Ʈ(�߿� FileIO �ʱ�ȭ)
			static Document create(const char* filename);
			///�ε�� ���ڿ����� ����Ʈ��Ʈ
			static Document create(const char* data, int size);
			////�Ѹ� Element ��
			Element root();
			////�Ѹ� Element ��
			ConstElement root() const;
			///�ε��� �Ϸ�Ǿ����� ����
			bool isReady();
			///�б� �� ������ �߻��ߴ°�
			bool isError() const;
			///��¥ xml �ؽ�Ʈ ����
			void convertToString(string* out) const;
			///��¥ xml ���� ����
			void convertToBinary(Array< char >* out) const;
			///��¥ xml ���� ���� (�ʿ� FileIO �ʱ�ȭ)
			void write(const char* filename, bool isBinary = false) const;
			///�ڽļ��ٰ�
			int childNumber() const;
			///�ڽİ���
			Element child(int i);
			///�ڽİ���
			ConstElement child(int i) const;

			//���� �ǽ����� �ʾƵ� �ȴ�
			Document();
			Document(const Document&);
			~Document();
			operator void* () const;
			Document& operator=(const Document&);
			void release();
			bool operator==(const Document&) const;
			bool operator!=(const Document&) const;

			class Impl;
		private:
			Impl* mImpl;
		};

		//const��
		class ConstDocument {
		public:
			///�Ѹ� Element ��
			ConstElement root() const;
			///�б� �� ������ �߻��ߴ°�
			bool isError() const;
			///��¥ xml �ؽ�Ʈ ����
			void convertToString(string* out) const;
			///��¥ xml ���� ����
			void convertToBinary(Array< char >* out) const;
			///��¥ xml ���� ���� (�ʿ� FileIO �ʱ�ȭ)
			void write(const char* filename, bool isBinary = false) const;
			///�ڽļ��ٰ�
			int childNumber() const;
			///�ڽİ���
			ConstElement child(int i) const;

			//���� ���� ��
			ConstDocument();
			ConstDocument(const Document&);
			~ConstDocument();
			ConstDocument& operator=(const Document&);
			void release();
			operator void* () const;
			bool operator==(const Document&) const;
			bool operator!=(const Document&) const;
		private:
			Document mBase;
		};

	} //namespace PseudoXml
} //namespace GameLib

#endif