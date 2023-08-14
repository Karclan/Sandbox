#pragma once

#include "format.h"
#include <fmt/core.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "entt/core/fwd.hpp"
#include "../Core/CoreRegistry.h"

template <> struct fmt::formatter < glm::vec2 >
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse( format_parse_context & ctx ) -> format_parse_context::iterator
	{
		// [ctx.begin(), ctx.end()) is a character range that contains a part of
		// the format string starting from the format specifications to be parsed,
		// e.g. in
		//
		//   fmt::format("{:f} - point of interest", point{1, 2});
		//
		// the range will contain "f} - point of interest". The formatter should
		// parse specifiers until '}' or the end of the range. In this example
		// the formatter should parse the 'f' specifier and return an iterator
		// pointing to '}'.
		// Please also note that this character range may be empty, in case of
		// the "{}" format string, so therefore you should check ctx.begin()
		// for equality with ctx.end().

		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		if ( it != end && ( *it == 'f' || *it == 'e' ) ) presentation = *it++;

		// Check if reached the end of the range:
		if ( it != end && *it != '}' ) ctx.on_error( "invalid format" );

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format( const glm::vec2 & vec2, format_context & ctx ) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f'
			? fmt::format_to( ctx.out(), "({:.3f}, {:.3f})", vec2.x, vec2.y )
			: fmt::format_to( ctx.out(), "({:.3e}, {:.3e})", vec2.x, vec2.y );
	}

};

template <> struct fmt::formatter < glm::vec3 >
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse( format_parse_context & ctx ) -> format_parse_context::iterator
	{
		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		if ( it != end && ( *it == 'f' || *it == 'e' ) ) presentation = *it++;

		// Check if reached the end of the range:
		if ( it != end && *it != '}' ) ctx.on_error( "invalid format" );

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format( const glm::vec3 & vec3, format_context & ctx ) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f'
			? fmt::format_to( ctx.out(), "({:.3f}, {:.3f}, {:.3f})", vec3.x, vec3.y, vec3.z )
			: fmt::format_to( ctx.out(), "({:.3e}, {:.3e}, {:.3e})", vec3.x, vec3.y, vec3.z );
	}
};

template <> struct fmt::formatter < glm::vec4 >
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse( format_parse_context & ctx ) -> format_parse_context::iterator
	{
		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		if ( it != end && ( *it == 'f' || *it == 'e' ) ) presentation = *it++;

		// Check if reached the end of the range:
		if ( it != end && *it != '}' ) ctx.on_error( "invalid format" );

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format( const glm::vec4 & vec4, format_context & ctx ) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f'
			? fmt::format_to( ctx.out(), "({:.3f}, {:.3f}, {:.3f}, {:.3f})", vec4.x, vec4.y, vec4.z, vec4.w )
			: fmt::format_to( ctx.out(), "({:.3e}, {:.3e}, {:.3e}, {:.3e})", vec4.x, vec4.y, vec4.z, vec4.w );
	}
};

template <> struct fmt::formatter < glm::dvec2 >
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse( format_parse_context & ctx ) -> format_parse_context::iterator
	{
		// [ctx.begin(), ctx.end()) is a character range that contains a part of
		// the format string starting from the format specifications to be parsed,
		// e.g. in
		//
		//   fmt::format("{:f} - point of interest", point{1, 2});
		//
		// the range will contain "f} - point of interest". The formatter should
		// parse specifiers until '}' or the end of the range. In this example
		// the formatter should parse the 'f' specifier and return an iterator
		// pointing to '}'.
		// Please also note that this character range may be empty, in case of
		// the "{}" format string, so therefore you should check ctx.begin()
		// for equality with ctx.end().

		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		if ( it != end && ( *it == 'f' || *it == 'e' ) ) presentation = *it++;

		// Check if reached the end of the range:
		if ( it != end && *it != '}' ) ctx.on_error( "invalid format" );

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format( const glm::dvec2 & vec2, format_context & ctx ) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f'
			? fmt::format_to( ctx.out(), "({:.3f}, {:.3f})", vec2.x, vec2.y )
			: fmt::format_to( ctx.out(), "({:.3e}, {:.3e})", vec2.x, vec2.y );
	}

};

template <> struct fmt::formatter < glm::dvec3 >
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse( format_parse_context & ctx ) -> format_parse_context::iterator
	{
		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		if ( it != end && ( *it == 'f' || *it == 'e' ) ) presentation = *it++;

		// Check if reached the end of the range:
		if ( it != end && *it != '}' ) ctx.on_error( "invalid format" );

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format( const glm::dvec3 & vec3, format_context & ctx ) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f'
			? fmt::format_to( ctx.out(), "({:.3f}, {:.3f}, {:.3f})", vec3.x, vec3.y, vec3.z )
			: fmt::format_to( ctx.out(), "({:.3e}, {:.3e}, {:.3e})", vec3.x, vec3.y, vec3.z );
	}
};

template <> struct fmt::formatter < glm::dvec4 >
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse( format_parse_context & ctx ) -> format_parse_context::iterator
	{
		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		if ( it != end && ( *it == 'f' || *it == 'e' ) ) presentation = *it++;

		// Check if reached the end of the range:
		if ( it != end && *it != '}' ) ctx.on_error( "invalid format" );

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format( const glm::dvec4 & vec4, format_context & ctx ) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f'
			? fmt::format_to( ctx.out(), "({:.3f}, {:.3f}, {:.3f}, {:.3f})", vec4.x, vec4.y, vec4.z, vec4.w )
			: fmt::format_to( ctx.out(), "({:.3e}, {:.3e}, {:.3e}, {:.3e})", vec4.x, vec4.y, vec4.z, vec4.w );
	}
};

template <> struct fmt::formatter < glm::ivec2 >
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse( format_parse_context & ctx ) -> format_parse_context::iterator
	{
		// [ctx.begin(), ctx.end()) is a character range that contains a part of
		// the format string starting from the format specifications to be parsed,
		// e.g. in
		//
		//   fmt::format("{:f} - point of interest", point{1, 2});
		//
		// the range will contain "f} - point of interest". The formatter should
		// parse specifiers until '}' or the end of the range. In this example
		// the formatter should parse the 'f' specifier and return an iterator
		// pointing to '}'.
		// Please also note that this character range may be empty, in case of
		// the "{}" format string, so therefore you should check ctx.begin()
		// for equality with ctx.end().

		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		if ( it != end && ( *it == 'f' || *it == 'e' ) ) presentation = *it++;

		// Check if reached the end of the range:
		if ( it != end && *it != '}' ) ctx.on_error( "invalid format" );

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format( const glm::ivec2 & vec2, format_context & ctx ) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f'
			? fmt::format_to( ctx.out(), "({}, {})", vec2.x, vec2.y )
			: fmt::format_to( ctx.out(), "({}, {})", vec2.x, vec2.y );
	}

};

template <> struct fmt::formatter < glm::mat3 >
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse( format_parse_context & ctx ) -> format_parse_context::iterator
	{
		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		if ( it != end && ( *it == 'f' || *it == 'e' ) ) presentation = *it++;

		// Check if reached the end of the range:
		if ( it != end && *it != '}' ) ctx.on_error( "invalid format" );

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format( const glm::mat3 & mat3, format_context & ctx ) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f'
			? fmt::format_to( ctx.out(), "({: .3f} | {: .3f} | {: .3f})\
										  \n({: .3f} | {: .3f} | {: .3f})\
										  \n({: .3f} | {: .3f} | {: .3f})"
							  , mat3[0][0], mat3[0][1], mat3[0][2],
							  mat3[1][0], mat3[1][1], mat3[1][2],
							  mat3[2][0], mat3[2][1], mat3[2][2] )
			: fmt::format_to( ctx.out(), "({: .3f} | {: .3f} | {: .3f})\
										  \n({: .3f} | {: .3f} | {: .3f})\
										  \n({: .3f} | {: .3f} | {: .3f})",
							  mat3[0][0], mat3[0][1], mat3[0][2],
							  mat3[1][0], mat3[1][1], mat3[1][2],
							  mat3[2][0], mat3[2][1], mat3[2][2]
			);
	}
};

template <> struct fmt::formatter < glm::mat4 >
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse( format_parse_context & ctx ) -> format_parse_context::iterator
	{
		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		if ( it != end && ( *it == 'f' || *it == 'e' ) ) presentation = *it++;

		// Check if reached the end of the range:
		if ( it != end && *it != '}' ) ctx.on_error( "invalid format" );

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format( const glm::mat4 & mat4, format_context & ctx ) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f'
			? fmt::format_to( ctx.out(), "({: .3f} | {: .3f} | {: .3f} | {: .3f})\
										  \n({: .3f} | {: .3f} | {: .3f} | {: .3f})\
										  \n({: .3f} | {: .3f} | {: .3f} | {: .3f})\
										  \n({: .3f} | {: .3f} | {: .3f} | {: .3f})"
							  , mat4[0][0], mat4[0][1], mat4[0][2], mat4[0][3],
							  mat4[1][0], mat4[1][1], mat4[1][2], mat4[1][3],
							  mat4[2][0], mat4[2][1], mat4[2][2], mat4[3][2],
							  mat4[3][0], mat4[3][1], mat4[3][2], mat4[3][3] )
			: fmt::format_to( ctx.out(), "({: .3f} | {: .3f} | {: .3f} | {: .3f})\
										  \n({: .3f} | {: .3f} | {: .3f} | {: .3f})\
										  \n({: .3f} | {: .3f} | {: .3f} | {: .3f})\
										  \n({: .3f} | {: .3f} | {: .3f} | {: .3f})",
							  mat4[0][0], mat4[0][1], mat4[0][2], mat4[0][3],
							  mat4[1][0], mat4[1][1], mat4[1][2], mat4[1][3],
							  mat4[2][0], mat4[2][1], mat4[2][2], mat4[3][2],
							  mat4[3][0], mat4[3][1], mat4[3][2], mat4[3][3]
			);
	}
};

template <> struct fmt::formatter < Core::Entity >
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse( format_parse_context & ctx ) -> format_parse_context::iterator
	{
		// Parse the presentation format and store it in the formatter:
		auto it = ctx.begin(), end = ctx.end();
		if ( it != end && ( *it == 'f' || *it == 'e' ) ) presentation = *it++;

		// Check if reached the end of the range:
		if ( it != end && *it != '}' ) ctx.on_error( "invalid format" );

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format( const Core::Entity & entity, format_context & ctx ) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f'
			? fmt::format_to( ctx.out(), "({})", static_cast< entt::id_type >( entity ) )
			: fmt::format_to( ctx.out(), "({})", static_cast< entt::id_type >( entity ) )
			;
	}
};
