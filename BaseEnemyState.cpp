#include "BaseEnemyState.h"
#include <windows.h>

void BaseEnemyState::DebugLog() {
#ifdef DEBUG

	OutputDebugStringA(name_.c_str());

#endif // DEBUG

}
