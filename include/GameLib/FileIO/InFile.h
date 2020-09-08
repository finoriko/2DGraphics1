#ifndef INCLUDED_GAMELIB_FILEIO_INFILE_H
#define INCLUDED_GAMELIB_FILEIO_INFILE_H

namespace GameLib {
	namespace FileIO {

		///�б� ���� �ڵ�
		class InFile {
		public:
			/// ���� �ε带 �����ϴ�
			static InFile create(const char* filename);
			/// �ε� �뷮 ȹ��
			int size() const;
			///�ε峻��ȹ��
			const char* data() const;
			//// �ε� ������?
			bool isFinished() const;
			//// ������ ����?
			bool isError() const;
			//��ȹ�Լ���
			int getInt(int position) const;
			unsigned getUnsigned(int position) const;
			short getShort(int position) const;
			unsigned short getUnsignedShort(int position) const;

			//���� ����ڴ� �ǽ����� �ʴ� �Լ���
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
