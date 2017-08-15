#include "Scene_Chili.h"
#include "ChiliMath.h"
#include <random>

namespace dx = DirectX;


Scene_Chili::Scene_Chili( Graphics &Gfx, MainWindow &Wnd )
	:
	gfx( Gfx ),
	wnd( Wnd )
{
	std::mt19937 rng( std::random_device{}( ) );
	std::uniform_real_distribution<float> distX( 0.0f, float( gfx.ScreenWidth - 1 ) );
	std::uniform_real_distribution<float> distY( 0.0f, float( gfx.ScreenHeight - 1 ) );
	std::normal_distribution<float> distAnglularVelocity( PI, PI / 2.0f );

	for( size_t i = 0; i < positions.size(); i++ )
	{
		positions[ i ] = { distX( rng ), distY( rng ) };
		angularVelocities[ i ] = distAnglularVelocity( rng );
	}
}

void Scene_Chili::UpdateModel( float DeltaTime )
{
	t += DeltaTime;
}

void Scene_Chili::ComposeFrame()
{
	auto batch = gfx.MakeSpriteBatch();

	batch.Begin( dx::SpriteSortMode_Deferred,
				 gfx.GetStates().NonPremultiplied(),
				 gfx.GetStates().PointClamp() );
	for( size_t i = 0; i < positions.size(); i++ )
	{
		const float angle = wrap_angle( t * angularVelocities[ i ] );
		marle.Draw( batch, positions[ i ], angle );
	}
	batch.End();
}
