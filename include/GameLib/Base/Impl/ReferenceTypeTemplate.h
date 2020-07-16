// 이것은 클래스를 쓰는 사람을 위한 것으로, 유저가 인크루드 하는 일은 없다.
//참조 카운트형 클래스에서는 반드시 가지고 있는 함수라고 하는 것이 있는데
//이것을 쓰는 것을 편하게 한다.
//#define TYPE Actual Type
// 라고 쓰고 인크루드 하는 것.물론 Actual Type은 진짜 형의 이름이다.

TYPE::TYPE() : mImpl(0) {
}

TYPE::TYPE(const TYPE& o) : mImpl(o.mImpl) {
	if (mImpl) {
		mImpl->refer();
	}
}

TYPE::~TYPE() {
	release();
}

void TYPE::release() {
	if (mImpl) {
		mImpl->release();
		if (mImpl->referenceCount() == 0) {
			SAFE_DELETE(mImpl);
		}
	}
	mImpl = 0;
}

TYPE& TYPE::operator=(const TYPE& o) {
	release();
	mImpl = o.mImpl;
	if (mImpl) {
		mImpl->refer();
	}
	return *this;
}

bool TYPE::operator==(const TYPE& o) const {
	return (mImpl == o.mImpl);
}

bool TYPE::operator!=(const TYPE& o) const {
	return (mImpl != o.mImpl);
}

TYPE::operator void* () const {
	return (mImpl == 0) ? 0 : reinterpret_cast<void*>(~0);
}

#undef TYPE