
#include <string.h>
#include <iostream>

int main(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {
			return 0;
		}
	}

	return 0;
}
