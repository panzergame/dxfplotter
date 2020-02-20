#include <iostream>

int main(int argc, char **argv)
{
	const int nbcontrol = atoi(argv[1]);
	const int midcontrol = nbcontrol / 2;
	const int lastcontrol = nbcontrol - 1;
	const int nbpairs = nbcontrol - 1;

	int size = nbcontrol;
	if (nbcontrol > 4) {
		size += 2; // Point deuxième et penultième
		size += (nbpairs - 4) * 2;
	}

	const int last = size - 1;
	const int mid = size / 2;

	std::cout << "nb bezier " << size << std::endl;

	std::cout << "=============" << std::endl;
	
	// Copy of begin and end raw points 0 1 2 ... -3 -2 -1 to 0 1 3 ... -4 -2 -1
	for (int src = 0; src < 2; ++src) {
		std::cout << src << " " << src << std::endl;
		std::cout << (lastcontrol - src) << " " << (last - src) << std::endl;
	}

	std::cout << "=============" << std::endl;

	// Copy of remaining raw points from 3 ... -3 to 6 9 12 ... -13 -10 -7 (every 3 with 6 point distance from begin and end).
	for (int src = 2, dst = 3; src < (nbcontrol - 2); ++src, dst += 3) {
		std::cout << src << " " << dst << std::endl;
	}

	std::cout << "=============" << std::endl;

	if (nbcontrol > 4) {
		// Copy half points 2 and -3
		std::cout << "(" << 1 << "," << 2 << ") " << 2 << std::endl;
		std::cout << "(" << (lastcontrol - 2) << "," << (lastcontrol - 1) << ") " << (last - 2) << std::endl;
	}

	std::cout << "=============" << std::endl;

	// Copy third point 2 ... -3 to (4 5) (7 8) .. (-9 -8) (-6 -5)
	for (int src = 2, dst = 4; src < (nbcontrol - 3); ++src, dst += 3) {
		std::cout << "(" << src << "," << (src + 1) << ") " << dst << " " << (dst + 1) << std::endl;
	}
}
