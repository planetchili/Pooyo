#include "Entity.h"

Entity::Entity( const float2 & Pos )
	:
	m_position( Pos )
{
}

void Entity::Update( float DeltaTime )
{
	m_position.y += ( 60.f * DeltaTime );
}

void Entity::SetPosition( const float2 & Pos )
{
	m_position = Pos;
}

float2 Entity::GetPosition() const
{
	return m_position;
}
