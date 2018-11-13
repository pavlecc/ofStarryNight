#pragma once

#ifdef _WIN32
#define PLATFORM_SWITCH(_win32Choice, _appleChoice) _win32Choice
#elif __APPLE__
#define PLATFORM_SWITCH(_win32Choice, _appleChoice) _appleChoice
#else
#error "Unsupported compiler"
#endif
