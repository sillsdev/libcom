#include "Hacks.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{
	std::cerr.sync_with_stdio(false);
	std::wcerr.sync_with_stdio(false);
	OutputDebugString(L"WideWideWide\n");
	OutputDebugString("NarrowNarrowNarrow\n");
	OLECHAR buf[] = {'O', 'l', 'e', 'O', 'l', 'e', '\n', 0};
	OutputDebugString(buf);
}
