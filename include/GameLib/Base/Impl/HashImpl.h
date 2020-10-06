#ifndef INCLUDED_GAMELIB_HASHIMPL_H
#define INCLUDED_GAMELIB_HASHIMPL_H

#include "GameLib/Base/Hash.h"
#include "GameLib/Base/RefString.h"
#include <string>


namespace GameLib {
	using namespace std;

	//표준판
	template< class T > inline unsigned Hash< T >::value(const T& v, unsigned m) const {
		const unsigned char* p = reinterpret_cast<const unsigned char*>(&v);
		unsigned h = 0;
		for (int i = 0; i < sizeof(T); ++i) {
			h *= 137;
			h += p[i];
		}
		h %= m;
		return h;
	}

	template< class T > inline bool Hash< T >::isEqual(const T& a, const T& b) const {
		return (a == b);
	}

	//포인터용 특별판
	template<> inline bool Hash< const char* >::isEqual(const char* const& a, const char* const& b) const {
		const char* pa = a;
		const char* pb = b;

		while (*pa == *pb) { //한 쪽이 먼저 끝나 버릴 때도 검출할 수 있다
			if (*pa == '\0') { //여기까지 똑같고 동시에 0이 되었다는 것은 똑같다는 것이다.
				return true;
			}
			++pa;
			++pb;
		}
		return false;
	}

	template<> inline bool Hash< const string* >::isEqual(const string* const& a, const string* const& b) const {
		if (a->size() != b->size()) { //없어도 되지만, 이것으로 빨리 끝날 가능성은 높다.
			return false;
		}
		//사이즈는 똑같다.
		const char* pa = a->c_str();
		const char* pb = b->c_str();

		while (*pa == *pb) { //한 쪽이 먼저 끝나 버릴 때도 검출할 수 있다
			if (*pa == '\0') { //여기까지 똑같고 동시에 0이 되었다는 것은 똑같다는 것이다.
				return true;
			}
			++pa;
			++pb;
		}
		return false;
	}

	template<> inline bool Hash< const RefString* >::isEqual(const RefString* const& a, const RefString* const& b) const {
		if (a->size() != b->size()) { //없어도 되지만, 이것으로 빨리 끝날 가능성은 높다.
			return false;
		}
		//사이즈는 똑같다.
		const char* pa = a->get();
		const char* pb = b->get();

		while (*pa == *pb) { //한 쪽이 먼저 끝나 버릴 때도 검출할 수 있다
			if (*pa == '\0') { //여기까지 똑같고 동시에 0이 되었다는 것은 똑같다는 것이다.
				return true;
			}
			++pa;
			++pb;
		}
		return false;
	}

	//특별버전 한 벌 준비
	template<> inline unsigned Hash< int >::value(const int& v, unsigned m) const {
		return static_cast<unsigned>(v) % m;
	}

	template<> inline unsigned Hash< unsigned > ::value(const unsigned& v, unsigned m) const {
		return v % m;
	}

	template<> inline unsigned Hash< long >::value(const long& v, unsigned m) const {
		return static_cast<unsigned>(v) % m;
	}

	template<> inline unsigned Hash< unsigned long >::value(const unsigned long& v, unsigned m) const {
		return static_cast<unsigned>(v) % m;
	}

	template<> inline unsigned Hash< short >::value(const short& v, unsigned m) const {
		return v % m;
	}

	template<> inline unsigned Hash< unsigned short >::value(const unsigned short& v, unsigned m) const {
		return v % m;
	}

	template<> inline unsigned Hash< char >::value(const char& v, unsigned m) const {
		return v % m;
	}

	template<> inline unsigned Hash< unsigned char >::value(const unsigned char& v, unsigned m) const {
		return v % m;
	}

	template<> inline unsigned Hash< float >::value(const float& v, unsigned m) const {
		const unsigned* p = reinterpret_cast<const unsigned*>(&v);
		return *p % m;
	}

	template<> inline unsigned Hash< double >::value(const double& v, unsigned m) const {
		const unsigned* p = reinterpret_cast<const unsigned*>(&v);
		unsigned h = p[0] * 137 + p[1];
		h %= m;
		return h;
	}

	template<> inline unsigned Hash< const char* >::value(const char* const& v, unsigned m) const {
		const unsigned char* p = reinterpret_cast<const unsigned char*>(v);
		unsigned h = 0;
		for (int i = 0; p[i] != '\0'; ++i) {
			h *= 137;
			h += p[i];
		}
		h %= m;
		return h;
	}

	template<> inline unsigned Hash< string >::value(const string& v, unsigned m) const {
		const unsigned char* p = reinterpret_cast<const unsigned char*>(v.c_str());
		unsigned h = 0;
		for (int i = 0; p[i] != '\0'; ++i) {
			h *= 137;
			h += p[i];
		}
		h %= m;
		return h;
	}

	template<> inline unsigned Hash< const string* >::value(const string* const& v, unsigned m) const {
		const unsigned char* p = reinterpret_cast<const unsigned char*>(v->c_str());
		unsigned h = 0;
		for (int i = 0; p[i] != '\0'; ++i) {
			h *= 137;
			h += p[i];
		}
		h %= m;
		return h;
	}

	template<> inline unsigned Hash< RefString >::value(const RefString& v, unsigned m) const {
		const unsigned char* p = reinterpret_cast<const unsigned char*>(v.get());
		unsigned h = 0;
		for (int i = 0; i < v.size(); ++i) {
			h *= 137;
			h += p[i];
		}
		h %= m;
		return h;
	}

	template<> inline unsigned Hash< const RefString* >::value(const RefString* const& v, unsigned m) const {
		const unsigned char* p = reinterpret_cast<const unsigned char*>(v->get());
		unsigned h = 0;
		for (int i = 0; i < v->size(); ++i) {
			h *= 137;
			h += p[i];
		}
		h %= m;
		return h;
	}

} //namespace GameLib

#endif
