#pragma once

#include <DirectXMath.h>

using float2 = DirectX::XMFLOAT2;

class Entity
{
public:
	Entity() = default;
	Entity( const float2 &Pos );
	virtual ~Entity() = default;

	virtual void Update( float DeltaTime );

	void SetPosition( const float2 &Pos );
	float2 GetPosition()const;

protected:
	float2 m_position;
};

