#ifndef INCLUDED_GAMELIB_SOUND_WAVE_H
#define INCLUDED_GAMELIB_SOUND_WAVE_H

namespace GameLib {
	namespace Sound {

		class Wave {
		public:
			static Wave create(const char* filename);
			///파형 직입력(8bit)
			static Wave create(const unsigned char* data, int size, int frequency, int channelNumber);
			///파형 직입력(16bit)
			static Wave create(const short* data, int size, int frequency, int channelNumber);
			bool isReady(); //로드 끝났어? (const가 아닌 건 안에서 처리하기 위해)
			bool isError() const; //에러가 나지 않았나?

			//이하 사용자는 의식하지 않는 함수군
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
