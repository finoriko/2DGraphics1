#ifndef INCLUDED_GAMELIB_THREADING_SEMAPHORE_H
#define INCLUDED_GAMELIB_THREADING_SEMAPHORE_H

namespace GameLib {
	namespace Threading {

		///정해진 인원수만큼 동시에 잠글 수 있는 Mutex.
		class Semaphore {
		public:
			//인수의 초기값으로 생성.max Count는 0이면 start Count가 된다.
			static Semaphore create(int startCount, int maxCount = 0);
			///카운트를 1 줄이려고 한다.벌써 0이면 활용되지 않는다
			void decrease();
			// 카운트를 늘린다.인수를 생략하면 하나씩.
			void increase(int value = 1);

			Semaphore();
			~Semaphore();
			Semaphore(const Semaphore&);
			operator void* () const;
			Semaphore& operator=(const Semaphore&);
			void release();
			bool operator==(const Semaphore&) const;
			bool operator!=(const Semaphore&) const;
		private:
			class Impl;
			Impl* mImpl;
		};

	} //namespace Threading
} //namespace GameLib

#endif
