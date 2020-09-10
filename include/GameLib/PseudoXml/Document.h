#ifndef INCLUDED_GAMELIB_PSEUDOXML_DOCUMENT_H
#define INCLUDED_GAMELIB_PSEUDOXML_DOCUMENT_H

#include <string>

namespace GameLib {
	template< class T > class Array;
	namespace PseudoXml {
		using namespace std;

		class Element;
		class ConstElement;

		///파일 액세스 기능을 가진 가짜 xml 근원 클래스
		class Document {
		public:
			///쓰기용으로 비어있는거 생성
			static Document create();
			///파일에서 컨스트럭트(중요 FileIO 초기화)
			static Document create(const char* filename);
			///로드된 문자열에서 컨스트럭트
			static Document create(const char* data, int size);
			////뿌리 Element 겟
			Element root();
			////뿌리 Element 겟
			ConstElement root() const;
			///로딩이 완료되었는지 여부
			bool isReady();
			///읽기 중 오류가 발생했는가
			bool isError() const;
			///가짜 xml 텍스트 생성
			void convertToString(string* out) const;
			///가짜 xml 이진 생성
			void convertToBinary(Array< char >* out) const;
			///가짜 xml 파일 생성 (필요 FileIO 초기화)
			void write(const char* filename, bool isBinary = false) const;
			///자식수겟겟
			int childNumber() const;
			///자식겟토
			Element child(int i);
			///자식겟토
			ConstElement child(int i) const;

			//이하 의식하지 않아도 된다
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

		//const판
		class ConstDocument {
		public:
			///뿌리 Element 겟
			ConstElement root() const;
			///읽기 중 오류가 발생했는가
			bool isError() const;
			///가짜 xml 텍스트 생성
			void convertToString(string* out) const;
			///가짜 xml 이진 생성
			void convertToBinary(Array< char >* out) const;
			///가짜 xml 파일 생성 (필요 FileIO 초기화)
			void write(const char* filename, bool isBinary = false) const;
			///자식수겟겟
			int childNumber() const;
			///자식겟토
			ConstElement child(int i) const;

			//이하 몰라도 돼
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