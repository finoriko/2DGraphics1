#ifndef INCLUDED_GAMELIB_SOUND_PLAYER_H
#define INCLUDED_GAMELIB_SOUND_PLAYER_H

namespace GameLib {
	namespace Sound {

		class Wave;

		class Player {
		public:
			static Player create(Wave);
			///��Ʈ���� ����� Player
			static Player create(int bitsPerSample, int bufferSizeInByte, int frequency, int channelNumber);

			///���. ���� ���� �����Ű�� �ʹٸ� true�� �ǳ��ش�.
			void play(bool looping = false);
			void stop();
			/// ���� ����(100�� �ִ�, 0�� �ּ�)
			void setVolume(int volume);
			bool isPlaying() const; // ��� ���Դϱ�?

			/// ��Ʈ���� ���ۿ� ����.��� ��ġ�� �ʹ� ������� �����ϸ� false�� ��ȯ�Ѵ�.�Ҹ��� ����.
			bool write(int position, const char* data, int size);
			//// ��Ʈ���� ���ۿ� ������ ��ִ´�.��� ��ġ�� �ʹ� ������� �����ϸ� false�� ��ȯ�Ѵ�.�Ҹ��� ����.
			bool fillSilence(int position, int size);
			////��� ���� ���� ��ġ�� ����Ʈ ������ ��ȯ
			int position() const;

			//���� ����ڴ� �ǽ����� �ʴ� �Լ���
			Player();
			Player(const Player&);
			~Player();
			operator void* () const;
			Player& operator=(const Player&);
			void release();
			bool operator==(const Player&) const;
			bool operator!=(const Player&) const;
		private:
			class Impl;
			Impl* mImpl;
		};

	} //namespace Sound
} //namespace GameLib

#endif
