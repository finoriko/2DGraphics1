#ifndef INCLUDED_GAMELIB_FILEIO_OUTFILE_H
#define INCLUDED_GAMELIB_FILEIO_OUTFILE_H

namespace GameLib {
	namespace FileIO {

		class OutFile {
		public:
			static OutFile create(const char* filename, const char* data, int size);
			///세이브 종료했나
			bool isFinished() const;
			///에러가 나왔나?
			bool isError() const;

			//이하 사용자는 의식하지 않는 함수군
			OutFile();
			OutFile(const OutFile&);
			~OutFile();
			operator void* () const;
			OutFile& operator=(const OutFile&);
			void release();
			bool operator==(const OutFile&) const;
			bool operator!=(const OutFile&) const;

			class Impl;
		private:
			Impl* mImpl;
		};

	} //namespace FileIO
} //namespace GameLib

#endif
