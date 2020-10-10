#ifndef INCLUDED_GAMELIB_PRIMENUMBER_H
#define INCLUDED_GAMELIB_PRIMENUMBER_H

namespace GameLib {
	namespace PrimeNumber {
		bool isPrimeNumber(int);
		//�μ����� ū �ּ� �Ҽ� ��ȯ
		int next(int);
		//�μ����� ���� �ִ� �Ҽ� ��ȯ
		int previous(int);
		//������ ������.�Ҽ��� ���ϴ� ����
		int sqrt(int);
	}
}

#endif
