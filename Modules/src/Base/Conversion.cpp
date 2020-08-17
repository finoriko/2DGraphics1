#include "GameLib/Base/Conversion.h"

namespace GameLib {

	namespace {

		int toString16(char* out, unsigned a, int n) { //n�� 16 ��ȭ���� ���� �ִ� �ڸ���
			static const char table[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F', };
			for (int i = 0; i < n; ++i) { //���ڸ����� ������� ó��
				int shift = (n - 1 - i) * 4;
				unsigned mask = 0xf << shift;
				unsigned t = a & mask;
				t >>= shift;
				out[i] = table[t];
			}
			return n;
		}

		//������ 0�� �״�� �����ش�.
		//�� ���̳ʽ� ������ �ȿ��� ���� �ʴ��� �ϸ� unsigned�� ������
		// ���̳ʽ� int�� ���� �ʰ� int�� ������ unsigned�� ���� �ʱ� �����̴�.
		void toString10Core(char* out, unsigned a, int n) { //n�� 10 ��ȭ���� ���� �ִ� �ڸ���.�ִ� 10
			//10�� ���
			static const int d[9] = {
				10,
				100,
				1000,
				10000,
				100000,
				1000000,
				10000000,
				100000000,
				1000000000,
			};
			//10���� ���� �������� �����ϰ� 10���� ������.
			for (int i = 0; i < n - 1; ++i) {
				int q = a / d[n - 2 - i];
				out[i] = static_cast<char>(q);
				a -= q * d[n - 2 - i];
			}
			out[n - 1] = static_cast<char>(a);
		}

		//�� ���̳ʽ� ������ �ȿ��� ���� �ʴ��� �ϸ� unsigned�� ������
		// ���̳ʽ� int�� ���� �ʰ� int�� ������ unsigned�� ���� �ʱ� �����̴�.
		int toString10(char* out, unsigned a, int n, bool minus) { //n�� 10 ��ȭ���� ���� �ִ� �ڸ���.�ִ� 10
			char* p = out;
			//���̳ʽ���� ���̳ʽ��� ���س���
			if (minus) {
				*p++ = '-';
			}
			char s[10]; //�߰� ����
			toString10Core(s, a, n);
			//0�� ��ŵ
			int begin = n;
			for (int i = 0; i < n; ++i) {
				if (s[i] != 0) {
					begin = i;
					break;
				}
			}
			if (begin == n) { //�� �ڸ��� ����. �� 0
				*p++ = '0';
			}
			else {
				for (int i = begin; i < n; ++i) {
					*p++ = '0' + s[i];
				}
			}
			return static_cast<int>(p - out);
		}

	} //namespace {}

	int toString10(char* out, char a) {
		bool minus = false;
		if (a < 0) {
			minus = true;
			a = -a;
		}
		return toString10(out, a, 3, minus);
	}

	int toString10(char* out, unsigned char a) {
		return toString10(out, a, 3, false);
	}

	int toString10(char* out, short a) {
		bool minus = false;
		if (a < 0) {
			minus = true;
			a = -a;
		}
		return toString10(out, a, 5, minus);
	}

	int toString10(char* out, unsigned short a) {
		return toString10(out, a, 5, false);
	}

	int toString10(char* out, int a) {
		bool minus = false;
		if (a < 0) {
			minus = true;
			a = -a;
		}
		return toString10(out, a, 10, minus);
	}

	int toString10(char* out, unsigned a) {
		return toString10(out, a, 10, false);
	}

	int toString16(char* out, char a) {
		return toString16(out, a, 2);
	}

	int toString16(char* out, unsigned char a) {
		return toString16(out, a, 2);
	}

	int toString16(char* out, short a) {
		return toString16(out, a, 4);
	}

	int toString16(char* out, unsigned short a) {
		return toString16(out, a, 4);
	}

	int toString16(char* out, int a) {
		return toString16(out, a, 8);
	}

	int toString16(char* out, unsigned a) {
		return toString16(out, a, 8);
	}

	int toString(char* out, float a, int precision) {
		char* p = out; //���� ������
		if (a < 0.f) { //��ȣ ó��
			*p++ = '-';
			a = -a;
		}
		unsigned o = *reinterpret_cast<unsigned*>(&a);
		int e = (o & 0x7f800000) >> 23;
		unsigned m = o & 0x007fffff; //���� ��Ʈ�� 1�� ���ϴ�
		//Ư���� ó��
		if (e == 0) {
			*p++ = '0';
			return static_cast<int>(p - out);
		}
		else if (e == 255) {
			if (m == 0) {
				*p++ = 'I';
				*p++ = 'n';
				*p++ = 'f';
			}
			else {
				*p++ = 'N';
				*p++ = 'a';
				*p++ = 'N';
			}
			return static_cast<int>(p - out);
		}
		e -= 127; //���� ���̾
		e -= 23; //�������� 2^23�ڸ����� �������� ����
		m += 0x00800000; //���� ��Ʈ�� 1�� ���ϴ�
		m <<= 8; //8��Ʈ ����Ʈ
		e -= 8; //�� ��ŭ �ڸ� �ٿ�

		int e10 = 0;
		if (e > 0) { //2�ŵ����������Ͽ� ����ϸ鼭 10���� ������.
			for (int i = 0; i < e; ++i) {
				if (m > 0x80000000) { //��ģ��!
					m /= 10;
					++e10;
					if (i + 2 < e) { //���� �� �� �̻� Ÿ�� ���⼭ ��ġ���.
						m <<= 2;
						i += 2;
					}
				}
				m <<= 1;
			}
		}
		else {
			e = -e;
			for (int i = 0; i < e; ++i) {
				if (m < 0x19000000) { //�ڸ����� �ø��ϴ�.
					m *= 10;
					--e10;
					if (i + 2 < e) { //���� 2ȸ �̻� �����Ѵٸ� ���⼭ �ع�����.
						m >>= 2;
						i += 2;
					}
				}
				m >>= 1;
			}
		}
		char s[10]; //�߰� ��� ����
		toString10Core(s, m, 10);
		//���� ������

		//���ΰ� 0�� �ƴϵ��� ������.������ ������ �ܼ�ȭ�Ǵ�
		int zeroEnd = 0;
		for (int i = 0; i < 10; ++i) {
			if (s[i] != 0) {
				break;
			}
			else {
				++zeroEnd;
			}
		}
		if (zeroEnd == 10) { //���� Ư���� ���� ������
			*p++ = '0';
			return static_cast<int>(p - out);
		}
		//�ű��
		for (int i = 0; i < 10 - zeroEnd; ++i) {
			s[i] = s[i + zeroEnd];
		}
		for (int i = 10 - zeroEnd; i < 10; ++i) {
			s[i] = 0;
		}
		e10 -= zeroEnd; //���� �и� ��� ����
		e10 += 9; //10���� �ڸ��� 1�� �ϴϱ� 9�ڸ� e�� �÷���

		//��� �ڸ���
		int n = (precision < 9) ? precision : 9;
		//�ݿø�
		bool carry = (s[n] >= 5);
		s[n] = 0;
		if (carry) {
			for (int i = n - 1; i >= 0; --i) {
				++s[i];
				if (s[i] == 10) {
					s[i] = 0;
				}
				else {
					carry = false;
					break;
				}
			}
			if (carry) { //�ִ� �ڸ������� ����ø��� �ִ�.���� �幰��
				s[0] = 1;
				for (int i = 1; i < 10; ++i) {
					s[i] = 0;
				}
				++e10;
			}
		}
		//������ �ڿ��� 0�� ���� �� ����ŭ n�� ���δ�.
		for (int i = n - 1; i > 0; --i) {
			if (s[i] == 0) {
				--n;
			}
			else {
				break;
			}
		}
		//e10�� [-2, precision)�� ������ e����� ������� �ʴ´�.�׷��� ª������ �����̴�.
		if ((e10 >= -2) && (e10 < precision)) {
			if (e10 >= 0) { //���� ���� ������ ���
				for (int i = 0; i < n; ++i) {
					*p++ = '0' + s[i];
					if ((i == e10) && (i != n - 1)) { //������ ����� �Ҽ��� ����
						*p++ = '.'; //�Ҽ��� ���
					}
				}
				//e�� Ŭ ��쿡�� ������ 0�� �ʿ���
				for (int i = n; i <= e10; ++i) {
					*p++ = '0';
				}
			}
			else { //���� ���� �Ҽ��� ���
				*p++ = '0';
				*p++ = '.';
				if (e10 == -2) {
					*p++ = '0';
				}
				for (int i = 0; i < n; ++i) {
					*p++ = '0' + s[i];
				}
			}
		}
		else { //e���
		   //���ڸ��� ���
			*p++ = '0' + s[0];
			//�Ҽ��� ���
			*p++ = '.';
			for (int i = 1; i < n; ++i) {
				*p++ = '0' + s[i];
			}
			if (e10 != 0) {
				*p++ = 'e';
				if (e10 < 0) {
					*p++ = '-';
					e10 = -e10;
				}
				int q = e10 / 10;
				if (q != 0) {
					*p++ = '0' + static_cast<char>(q);
				}
				e10 -= q * 10;
				*p++ = '0' + static_cast<char>(e10);
			}
		}
		return static_cast<int>(p - out);
	}

	int toString(char* out, double a, int precision) {
		return toString(out, static_cast<float>(a), precision); //TODO
	}


} //namespace GameLib

