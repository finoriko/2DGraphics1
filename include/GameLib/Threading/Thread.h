#ifndef INCLUDED_GAMELIB_THREADING_THREAD_H
#define INCLUDED_GAMELIB_THREADING_THREAD_H

namespace GameLib {
	namespace Threading {

		// ������ �߻��� ����
		/*!
		operator()�� �غ��� �Ļ��� �����,
		start() �ϸ� ������ ���۵ȴ�.
		
		<����!>
		��Ʈ���͸� �θ��� ������ wait()�� �θ��ų�, isFinished()�� true�� ��ȯ�� ������ �θ��ų�
		�� �� �ϳ��� �ʿ���.��� �ʵ� ���� �ʰ� ���ϸ� ASSERT �Ѵ�.
		*/
		class Thread {
		public:
			///�̰� ���� �����
			virtual void operator()() = 0;
			///���� ����.���� ���̶�� ���Ḧ ��ٷ� �����.
			void start();
			///���Ḧ ��ٸ���
			void wait();
			///�������� ����
			bool isFinished();
			///���� �θ����� �ʴ� ����Ʈ����
			Thread();
		protected: //������ ���� delete �� �� �����ϴ�.�Ļ��� delete �սô�.
			~Thread();
		private:
			class Impl;
			Impl* mImpl;
		};

	} //namespace Threading
} //namespace GameLib

#endif
