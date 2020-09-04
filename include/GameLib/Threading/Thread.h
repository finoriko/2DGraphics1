#ifndef INCLUDED_GAMELIB_THREADING_THREAD_H
#define INCLUDED_GAMELIB_THREADING_THREAD_H

namespace GameLib {
	namespace Threading {

		// 쓰레드 추상의 기저
		/*!
		operator()를 준비한 파생을 만들고,
		start() 하면 실행이 시작된다.
		
		<주의!>
		디스트럭터를 부르기 전에는 wait()를 부르거나, isFinished()가 true를 반환할 때까지 부르거나
		둘 중 하나가 필요해.어느 쪽도 하지 않고 퇴각하면 ASSERT 한다.
		*/
		class Thread {
		public:
			///이걸 직접 만드는
			virtual void operator()() = 0;
			///실행 시작.실행 중이라면 종료를 기다려 재실행.
			void start();
			///종료를 기다리다
			void wait();
			///끝났는지 묻다
			bool isFinished();
			///직접 부르지는 않는 컨스트럭터
			Thread();
		protected: //기저는 직접 delete 할 수 없습니다.파생을 delete 합시다.
			~Thread();
		private:
			class Impl;
			Impl* mImpl;
		};

	} //namespace Threading
} //namespace GameLib

#endif
