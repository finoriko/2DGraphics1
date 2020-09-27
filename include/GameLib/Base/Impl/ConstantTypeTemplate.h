//���� �������̽��� Const���� ���� �� ���ϰ� �ϱ� ���� ��.
//#define TYPE ActualType
//#define CONST_TYPE Actual ConstType
//��� ���� ���� ��Ŭ��� �� ��.���� Actual Type�� ��¥ ���� �̸��̴�.
// ������ ������ mBase ����

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