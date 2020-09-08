#ifndef INCLUDED_GAMELIB_FILEIO_MANAGER_H
#define INCLUDED_GAMELIB_FILEIO_MANAGER_H

#include <string>

namespace GameLib {
	namespace FileIO {

		class Manager {
		public:
			//아카이브 사용법
			enum AccessMode {
				MODE_ARCHIVE_FIRST, // 아카이브 우선.
				MODE_DIRECT_FIRST, //직접 액세스 우선.
				MODE_ARCHIVE_ONLY, //아카이브만.나눠줄 때는 이걸로 해두는 게 좋아.
			};
			///인스턴스를 얻다.그러나 단순히 Manager().foo()라고 써도 된다.
			static Manager instance();
			///핸들을 받지 않고 기입한다.성공 실패의 구별은 모른다.
			void write(const char* filename, const char* data, int size);
			///로드하고 있는 것을 리스트로 만들어 뱉어낸다.데뷔용. 개방 누락 여부를 조사하자.
			std::string createListString() const;
			///용량 한계 지정.warning을 넘으면 경고가, error를 넘으면 에러로 한다.표준값은 양쪽 2GB
			void setLimit(int warning, int error);
			///에러를 일으켰을 때에 멈출지 어떨지.표준은 true. 툴등에 멈춰서 곤란하면 false를 넣어라.
			void enableHaltOnError(bool);

			//이하 사용자는 의식하지 않는다
			static void create(
				const char** archiveNames = 0,
				int archiveNumber = 0,
				AccessMode = MODE_DIRECT_FIRST);
			static void destroy();
		};

	} //namespace Sound
} //namespace GameLib

#endif
