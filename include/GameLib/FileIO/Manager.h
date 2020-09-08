#ifndef INCLUDED_GAMELIB_FILEIO_MANAGER_H
#define INCLUDED_GAMELIB_FILEIO_MANAGER_H

#include <string>

namespace GameLib {
	namespace FileIO {

		class Manager {
		public:
			//��ī�̺� ����
			enum AccessMode {
				MODE_ARCHIVE_FIRST, // ��ī�̺� �켱.
				MODE_DIRECT_FIRST, //���� �׼��� �켱.
				MODE_ARCHIVE_ONLY, //��ī�̺길.������ ���� �̰ɷ� �صδ� �� ����.
			};
			///�ν��Ͻ��� ���.�׷��� �ܼ��� Manager().foo()��� �ᵵ �ȴ�.
			static Manager instance();
			///�ڵ��� ���� �ʰ� �����Ѵ�.���� ������ ������ �𸥴�.
			void write(const char* filename, const char* data, int size);
			///�ε��ϰ� �ִ� ���� ����Ʈ�� ����� ����.���߿�. ���� ���� ���θ� ��������.
			std::string createListString() const;
			///�뷮 �Ѱ� ����.warning�� ������ ���, error�� ������ ������ �Ѵ�.ǥ�ذ��� ���� 2GB
			void setLimit(int warning, int error);
			///������ �������� ���� ������ ���.ǥ���� true. ��� ���缭 ����ϸ� false�� �־��.
			void enableHaltOnError(bool);

			//���� ����ڴ� �ǽ����� �ʴ´�
			static void create(
				const char** archiveNames = 0,
				int archiveNumber = 0,
				AccessMode = MODE_DIRECT_FIRST);
			static void destroy();
		};

	} //namespace Sound
} //namespace GameLib

#endif
