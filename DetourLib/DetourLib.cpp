#define LIBRARY_EXPORTS

#include <windows.h>
#include "DetourLib.h"

LIBRARY_API void A::Foo()
{
	Sleep(2500);
}
