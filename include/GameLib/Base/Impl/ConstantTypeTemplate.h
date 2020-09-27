//참조 인터페이스형 Const판을 만들 때 편하게 하기 위한 것.
//#define TYPE ActualType
//#define CONST_TYPE Actual ConstType
//라고 쓰고 나서 인클루드 할 것.물론 Actual Type은 진짜 형의 이름이다.
// 구성원 변수는 mBase 고정

CONST_TYPE::CONST_TYPE() {
}

CONST_TYPE::CONST_TYPE(const TYPE& o) : mBase(o) {
}

CONST_TYPE::~CONST_TYPE() {
}

void CONST_TYPE::release() {
	mBase.release();
}

CONST_TYPE& CONST_TYPE::operator=(const TYPE& o) {
	mBase = o;
	return *this;
}

bool CONST_TYPE::operator==(const TYPE& o) const {
	return (mBase == o);
}

bool CONST_TYPE::operator!=(const TYPE& o) const {
	return (mBase != o);
}

CONST_TYPE::operator void* () const {
	return mBase.operator void* ();
}

#undef TYPE
#undef CONST_TYPE