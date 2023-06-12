#pragma once

#include <algorithm>

namespace Utils
{
	template<size_t N>
	struct StringLiteral
	{
		constexpr StringLiteral( const char( &str )[ N ] )
		{
			std::copy_n( str, N, Value );
		}
		char Value[ N ];
	};
}
