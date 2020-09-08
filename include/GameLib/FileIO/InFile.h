#ifndef INCLUDED_GAMELIB_FILEIO_INFILE_H
#define INCLUDED_GAMELIB_FILEIO_INFILE_H

namespace GameLib {
	namespace FileIO {

		///읽기 파일 핸들
		class InFile {
		public:
			/// 파일 로드를 시작하다
			static InFile create(const char* filename);
			/// 로드 용량 획득
			int size() const;
			///로드내용획득
			const char* data() const;
			//// 로드 끝났어?
			bool isFinished() const;
			//// 에러가 나죠?
			bool isError() const;
			//값획함수군
			int getInt(int position) const;
			unsigned getUnsigned(int position) const;
			short getShort(int position) const;
			unsigned short getUnsignedShort(int position) const;

			//이하 사용자는 의식하지 않는 함수군
			InFile();
			InFile(const InFile&);
			~InFile();
			operator void* () const;
			InFile& operator=(const InFile&);
			void release();
			bool operator==(const InFile&) const;
			bool operator!=(const InFile&) const;

			class Impl;
		private:
			Impl* mImpl;
		};

	} //namespace FileIO
} //namespace GameLib

#endif
