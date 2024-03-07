#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <ranges>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace vs = std::ranges::views;
namespace rng = std::ranges;

glm::u32 LCG( glm::u32 a, glm::u32 c, glm::u32 m, glm::u32 s ) { return ( a * s + c ) % m; }

glm::u32 next_lcg( glm::uvec4 prev = glm::uvec4 { 0 }, glm::u32 m = 65535 )
{
	glm::u32 next = 0;
	for ( glm::u8 i = 0 ; i < 4 ; ++i ) next = LCG( 22695477, next + 1, m, prev[ i ] );
	return next;
}

glm::u32 next_lcg_( glm::u32 prev = 0 )
{

	glm::u32 next = 0;
	next          = LCG( 22695477, next, 65535, prev );
	return LCG( 22695477, next, 65535, prev );
}

int main( int argc, char **argv )
{


	glm::uvec4 vec;

	if (argc != 5)
		{
			printf("Usage: %s <a> <b> <c> <d> 0 - 66535\n", argv[0]);
			return 1;
		}

	vec = {std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]), std::stoi(argv[4])};

	printf("%u", next_lcg( vec ));

	return 0;
}
