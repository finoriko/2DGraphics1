#include "GameLib/Base/Conversion.h"

namespace GameLib {

	namespace {

		int toString16(char* out, unsigned a, int n) { //n은 16 진화했을 때의 최대 자릿수
			static const char table[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F', };
			for (int i = 0; i < n; ++i) { //윗자리부터 순서대로 처리
				int shift = (n - 1 - i) * 4;
				unsigned mask = 0xf << shift;
				unsigned t = a & mask;
				t >>= shift;
				out[i] = table[t];
			}
			return n;
		}

		//선두의 0을 그대로 돌려준다.
		//왜 마이너스 판정을 안에서 하지 않느냐 하면 unsigned로 받으면
		// 마이너스 int가 들어가지 않고 int로 받으면 unsigned가 들어가지 않기 때문이다.
		void toString10Core(char* out, unsigned a, int n) { //n는 10 진화했을 때의 최대 자리수.최대 10
			//10진 상수
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
			//10으로 나눈 나머지를 저장하고 10으로 나눈다.
			for (int i = 0; i < n - 1; ++i) {
				int q = a / d[n - 2 - i];
				out[i] = static_cast<char>(q);
				a -= q * d[n - 2 - i];
			}
			out[n - 1] = static_cast<char>(a);
		}

		//왜 마이너스 판정을 안에서 하지 않느냐 하면 unsigned로 받으면
		// 마이너스 int가 들어가지 않고 int로 받으면 unsigned가 들어가지 않기 때문이다.
		int toString10(char* out, unsigned a, int n, bool minus) { //n는 10 진화했을 때의 최대 자리수.최대 10
			char* p = out;
			//마이너스라면 마이너스를 토해낸다
			if (minus) {
				*p++ = '-';
			}
			char s[10]; //중간 버퍼
			toString10Core(s, a, n);
			//0을 스킵
			int begin = n;
			for (int i = 0; i < n; ++i) {
				if (s[i] != 0) {
					begin = i;
					break;
				}
			}
			if (begin == n) { //한 자리도 없다. 즉 0
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
		char* p = out; //기입 포인터
		if (a < 0.f) { //부호 처리
			*p++ = '-';
			a = -a;
		}
		unsigned o = *reinterpret_cast<unsigned*>(&a);
		int e = (o & 0x7f800000) >> 23;
		unsigned m = o & 0x007fffff; //숨은 비트의 1을 더하다
		//특수수 처리
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
		e -= 127; //지수 바이어스
		e -= 23; //가수부의 2^23자리수를 지수에서 빼다
		m += 0x00800000; //숨은 비트의 1을 더하다
		m <<= 8; //8비트 시프트
		e -= 8; //그 만큼 자리 다운

		int e10 = 0;
		if (e > 0) { //2거듭제곱을곱하여 계산하면서 10으로 나누다.
			for (int i = 0; i < e; ++i) {
				if (m > 0x80000000) { //넘친다!
					m /= 10;
					++e10;
					if (i + 2 < e) { //아직 두 번 이상 타면 여기서 해치운다.
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
				if (m < 0x19000000) { //자릿수를 올립니다.
					m *= 10;
					--e10;
					if (i + 2 < e) { //아직 2회 이상 제외한다면 여기서 해버린다.
						m >>= 2;
						i += 2;
					}
				}
				m >>= 1;
			}
		}
		char s[10]; //중간 출력 버퍼
		toString10Core(s, m, 10);
		//이하 포맷팅

		//선두가 0이 아니도록 물리다.나중의 공정이 단순화되다
		int zeroEnd = 0;
		for (int i = 0; i < 10; ++i) {
			if (s[i] != 0) {
				break;
			}
			else {
				++zeroEnd;
			}
		}
		if (zeroEnd == 10) { //전부 특수한 경우라서 빠진다
			*p++ = '0';
			return static_cast<int>(p - out);
		}
		//옮기다
		for (int i = 0; i < 10 - zeroEnd; ++i) {
			s[i] = s[i + zeroEnd];
		}
		for (int i = 10 - zeroEnd; i < 10; ++i) {
			s[i] = 0;
		}
		e10 -= zeroEnd; //위로 밀린 몫수 감소
		e10 += 9; //10억의 자리를 1로 하니까 9자리 e에 플러스

		//출력 자리수
		int n = (precision < 9) ? precision : 9;
		//반올림
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
			if (carry) { //최대 자리수까지 끌어올리고 있다.극히 드물음
				s[0] = 1;
				for (int i = 1; i < 10; ++i) {
					s[i] = 0;
				}
				++e10;
			}
		}
		//다음에 뒤에서 0을 세어 그 수만큼 n을 줄인다.
		for (int i = n - 1; i > 0; --i) {
			if (s[i] == 0) {
				--n;
			}
			else {
				break;
			}
		}
		//e10이 [-2, precision)에 있으면 e기법은 사용하지 않는다.그래야 짧아지기 때문이다.
		if ((e10 >= -2) && (e10 < precision)) {
			if (e10 >= 0) { //제일 위가 정수인 경우
				for (int i = 0; i < n; ++i) {
					*p++ = '0' + s[i];
					if ((i == e10) && (i != n - 1)) { //마지막 수라면 소수점 무용
						*p++ = '.'; //소수점 출력
					}
				}
				//e가 클 경우에는 여분의 0이 필요함
				for (int i = n; i <= e10; ++i) {
					*p++ = '0';
				}
			}
			else { //제일 위가 소수인 경우
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
		else { //e기법
		   //한자릿수 출력
			*p++ = '0' + s[0];
			//소수점 출력
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

