#ifndef INCLUDED_GAMELIB_THREADING_MANAGER_H
#define INCLUDED_GAMELIB_THREADING_MANAGER_H

namespace GameLib {
	namespace Threading {

		class Manager {
		public:
			Manager();
			static Manager instance();

			//�ھ� �� ���(�� �ھ� ���̹Ƿ� ��ü�ʹ� �ٸ� �� ����)
			int getCoreNumber() const;

			//���� ����ڰ� �θ��� ���� ����
			static void create(int additionalThreadNumber);
			static void destroy();
		};

	} //namespace Threading
} //namespace GameLib

#endif
