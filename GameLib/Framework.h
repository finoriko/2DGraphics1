#ifndef INCLUDED_GAMELIB_FRAMEWORK_H
#define INCLUDED_GAMELIB_FRAMEWORK_H
namespace GameLib {
	class Framework
	{
	public:
		Framework();
		//����� ���� �Լ�.�̰��� ����.
		void update();
		///�ν��Ͻ� �ޱ�
		static Framework instance();
		///ȭ���� ��������
		int width() const;
		///�͸� ���� ��������
		int height() const;
		///VRAM �޾ƿ���
		unsigned* videoMemory();
		///Framework������ ������
		void requestEnd();
		///Framework�� �������� ������ �ִ��� �˾ƺ���
		bool isEndRequested() const;

		//�Ʒ� ���̺귯�� ����ڴ� ���� �ȴ�
		void start(void* windowHandle);
		void preUpdate();
		void postUpdate();
		static void create();
		static void destroy();
	};
	class StandardInput {
	public:
		StandardInput();
		StandardInput& operator>>(char&);
	private:
		bool mPrevEnterOn;
	};
	extern StandardInput cin;

} //namespace GameLib
#include "GameLib/Base/DebugStream.h"

#endif