#ifndef INCLUDED_GAMELIB_PRIMENUMBER_H
#define INCLUDED_GAMELIB_PRIMENUMBER_H

namespace GameLib {
	namespace PrimeNumber {
		bool isPrimeNumber(int);
		//인수보다 큰 최소 소수 반환
		int next(int);
		//인수보다 작은 최대 소수 반환
		int previous(int);
		//정수의 제곱근.소수점 이하는 절상
		int sqrt(int);
	}
}

#endif
