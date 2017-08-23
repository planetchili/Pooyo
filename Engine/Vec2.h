#pragma once

#include <cmath>
#include <DirectXMath.h>

using Vec2f = DirectX::XMFLOAT2;
using Vec2i = DirectX::XMINT2;

inline Vec2f operator+( const Vec2f &A, const Vec2f &B )
{
	return{ A.x + B.x, A.y + B.y };
}
inline Vec2f &operator+=( Vec2f &A, const Vec2f &B )
{	
	return A = A + B;
}
inline Vec2f operator-( const Vec2f &A, const Vec2f &B )
{
	return{ A.x - B.x, A.y - B.y };
}
inline Vec2f operator*( const Vec2f &A, const float S )
{
	return{ A.x * S, A.y * S };
}
inline float DotProduct( const Vec2f &A, const Vec2f &B )
{
	return( ( A.x * B.x ) + ( A.y * B.y ) );
}
inline float Length( const Vec2f &A )
{
	return std::sqrtf( DotProduct( A, A ) );
}
inline Vec2f Normalize( const Vec2f &A )
{
	const float len = Length( A );
	return ( len == 0.f ) ? A : A * ( 1.f / len );
}

inline Vec2i operator+( const Vec2i &A, const Vec2i &B )
{
	return{ A.x + B.x, A.y + B.y };
}
inline Vec2i &operator+=( Vec2i &A, const Vec2i &B )
{
	return A = A + B;
}
inline Vec2i operator-( const Vec2i &A, const Vec2i &B )
{
	return{ A.x - B.x, A.y - B.y };
}
inline Vec2i operator*( const Vec2i &A, const int S )
{
	return{ A.x * S, A.y * S };
}
