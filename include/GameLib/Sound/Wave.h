#ifndef INCLUDED_GAMELIB_SOUND_WAVE_H
#define INCLUDED_GAMELIB_SOUND_WAVE_H

namespace GameLib {
	namespace Sound {

		class Wave {
		public:
			static Wave create(const char* filename);
			///���� ���Է�(8bit)
			static Wave create(const unsigned char* data, int size, int frequency, int channelNumber);
			///���� ���Է�(16bit)
			static Wave create(const short* data, int size, int frequency, int channelNumber);
			bool isReady(); //�ε� ������? (const�� �ƴ� �� �ȿ��� ó���ϱ� ����)
			bool isError() const; //������ ���� �ʾҳ�?

			//���� ����ڴ� �ǽ����� �ʴ� �Լ���
			Wave();
			Wave(const Wave&);
			~Wave();
			operator void* () const;
			Wave& operator=(const Wave&);
			void release();
			bool operator==(const Wave&) const;
			bool operator!=(const Wave&) const;
		private:
			class Impl;
			friend class Player;
			Impl* mImpl;
		};

	} //namespace Sound
} //namespace GameLib

#endif
