#ifndef INCLUDED_GAMELIB_SOUND_PLAYER_H
#define INCLUDED_GAMELIB_SOUND_PLAYER_H

namespace GameLib {
	namespace Sound {

		class Wave;

		class Player {
		public:
			static Player create(Wave);
			///스트리밍 재생용 Player
			static Player create(int bitsPerSample, int bufferSizeInByte, int frequency, int channelNumber);

			///재생. 무한 루프 재생시키고 싶다면 true를 건네준다.
			void play(bool looping = false);
			void stop();
			/// 볼륨 설정(100이 최대, 0이 최소)
			void setVolume(int volume);
			bool isPlaying() const; // 재생 중입니까?

			/// 스트리밍 버퍼에 쓴다.재생 위치가 너무 가까워져 실패하면 false를 반환한다.소리가 날다.
			bool write(int position, const char* data, int size);
			//// 스트리밍 버퍼에 무음을 써넣는다.재생 위치가 너무 가까워져 실패하면 false를 반환한다.소리가 날다.
			bool fillSilence(int position, int size);
			////재생 중인 버퍼 위치를 바이트 단위로 반환
			int position() const;

			//이하 사용자는 의식하지 않는 함수군
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
