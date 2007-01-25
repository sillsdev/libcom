#include "COM.h"
#include <iostream>

int main(int argc, char** argv)
{
	GUID guid(true);
	
	std::cout << guid.str() << "\n";
}
