#ifndef INCLUDED_GAMELIB_MATH_RANDOM_H
#define INCLUDED_GAMELIB_MATH_RANDOM_H

namespace GameLib 
{
	namespace Math 
	{
		//난수생성클래스
		class Random
		{
		public:
			static Random create(int seed = -1);
			///Seed재설정
			void setSeed(int);
			///-0x80000000 - 0x7fffffff
			int getInt();
			///a-1까지[0,a)
			int getInt(int a);
			///a에서 b-1까지[a,b)
			int getInt(int a, int b);
			///0-1이지만 1은 포함되지 않는다.( 23bit)
			float getFloat();
			///a에서 b까지. 단 b는 포함하지 않는다.[a, b) (23bit)
			float getFloat(float a);
			///a에서 b까지. 단 b는 포함하지 않는다.[a, b) (23bit)
			float getFloat(float a, float b);

			//아래는 몰라도 된다
			Random();
			Random(const Random&);
			~Random();
			operator void* () const;
			Random& operator=(const Random&);
			void release();
			bool operator==(const Random&) const;
			bool operator!=(const Random&) const;
			bool operator<(const Random&) const;
		private:
			class Impl;
			Impl* mImpl;
		};
	} //namespace Math
} //namespace GameLib

#endif