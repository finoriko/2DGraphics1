#ifndef INCLUDED_GAMELIB_THREADING_SEMAPHORE_H
#define INCLUDED_GAMELIB_THREADING_SEMAPHORE_H

namespace GameLib {
	namespace Threading {

		///������ �ο�����ŭ ���ÿ� ��� �� �ִ� Mutex.
		class Semaphore {
		public:
			//�μ��� �ʱⰪ���� ����.max Count�� 0�̸� start Count�� �ȴ�.
			static Semaphore create(int startCount, int maxCount = 0);
			///ī��Ʈ�� 1 ���̷��� �Ѵ�.���� 0�̸� Ȱ����� �ʴ´�
			void decrease();
			// ī��Ʈ�� �ø���.�μ��� �����ϸ� �ϳ���.
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
