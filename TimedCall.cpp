#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> f, uint32_t time) 
: callBack(f), time_(time) {}

void TimedCall::Update() {

	if (isFinished_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinished_ = true;
		//コールバック関数の呼び出し
		callBack();
	}

}
