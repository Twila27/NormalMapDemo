#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Game/TheGame.hpp"
#include "Game/TheApp.hpp" //For input polling.
//Be sure to #include all forward declared classes' .hpp's.
//#include "Game/Ship.hpp" //Whatever represents the player.
//#include "Game/Bullet.hpp" //Etc.

#include <cstdlib>
#include <math.h>
#include <stdlib.h>

//Can add virtual key macros here.
//#define VK_SPACE 0x20 //Etc. F1 on macro name to pull up full MSDN list.

//-----------------------------------------------------------------------------
TheGame* g_theGame = nullptr;


//-----------------------------------------------------------------------------
//Add knob constants that aren't needed in .hpp, can still add there as extern.
//const int INITIAL_NUMBER_OF_ASTEROIDS = 6; //Example, used in constructor.
//const float ASTEROID_SPEED_CHANGE_ON_BREAKUP_SCALING_KNOB = 1.25f; //Etc.


//-----------------------------------------------------------------------------
TheGame::TheGame( )
{
	//Remember to initialize any pointers to initial values or to nullptr in here.
	// for ( int i = 0; i < INITIAL_NUMBER_OF_ASTEROIDS; i++ ) m_asteroids[ i ] = new Asteroid(...);
	// for ( int i = 0; i < MAX_NUMBER_OF_BULLETS; i++ ) m_bullets[ i ] = nullptr; //Etc.

	//Position player representation if camera isn't translated to player (i.e. if camera is fixed in world).
	//m_ship = new Ship( static_cast<float>( screenWidth ) / 2.f, static_cast<float>( screenHeight ) / 2.f ); 
}


//-----------------------------------------------------------------------------
TheGame::~TheGame( )
{
	//delete m_ship; //Whatever represents player.
	//for ( int i = 0; i < m_numBulletsAllocated; i++ ) delete m_bullets[ i ]; //Etc.
}


//-----------------------------------------------------------------------------
void TheGame::Update( float deltaSeconds )
{
	//m_ship->Update( deltaSeconds ); //Update player representation.

	// Example: Asteroids Update Loop, includes wave transition, collision, and input detection/handling:
	/*
	for ( int i = 0; i < m_numAsteroidsAllocated; i++ )
	{
	if ( m_asteroids[ i ]->IsAlive( ) )
	{
	m_asteroids[ i ]->Update( deltaSeconds );
	if ( m_ship->IsAlive( ) && m_ship->DoesOverlap( *m_asteroids[ i ] ) )
	{
	m_ship->StartDeathTimer( );
	m_asteroids[ i ]->SetIsAlive( false );
	delete m_asteroids[ i ];
	m_asteroids[ i ] = nullptr;
	if ( m_numAsteroidsAllocated > 1 ) //Fill gap in array.
	{
	m_asteroids[ i ] = m_asteroids[ m_numAsteroidsAllocated - 1 ];
	m_asteroids[ m_numAsteroidsAllocated - 1 ] = nullptr;
	}
	--m_numAsteroidsAllocated;
	}
	for ( int j = 0; j < m_numBulletsAllocated; j++ )
	{
	if ( m_bullets[ j ]->IsAlive( ) && m_asteroids[ i ] != nullptr && m_bullets[ j ]->DoesOverlap( *m_asteroids[ i ] ) )
	{
	m_bullets[ j ]->SetIsAlive( false );
	if ( m_asteroids[ i ]->GetSize( ) == 1 ) //Smallest size.
	{
	m_asteroids[ i ]->SetIsAlive( false );
	delete m_asteroids[ i ];
	m_asteroids[ i ] = nullptr;
	if ( m_numAsteroidsAllocated > 1 ) //Fill gap in array.
	{
	m_asteroids[ i ] = m_asteroids[ m_numAsteroidsAllocated - 1 ];
	m_asteroids[ m_numAsteroidsAllocated - 1 ] = nullptr;
	}
	--m_numAsteroidsAllocated;
	}
	else if ( m_numAsteroidsAllocated < MAX_NUMBER_OF_ASTEROIDS )
	{
	m_asteroids[ i ]->DecreaseSize( );
	m_asteroids[ i ]->SetVelocity( m_asteroids[ i ]->GetVelocity( ) * ASTEROID_SPEED_CHANGE_ON_BREAKUP_SCALING_KNOB );
	//Break asteroid by making one a tinier version of itself, and spawning off a new one with opposite velocity.
	//Maybe lower its velocity?
	//Make new asteroid.
	m_asteroids[ m_numAsteroidsAllocated ] = new Asteroid( );
	m_asteroids[ m_numAsteroidsAllocated ]->SetPosition( m_asteroids[ i ]->GetPosition( ) );
	m_asteroids[ m_numAsteroidsAllocated ]->SetVelocity( m_asteroids[ i ]->GetVelocity( ) * -1.f );
	m_asteroids[ m_numAsteroidsAllocated ]->SetSize( m_asteroids[ i ]->GetSize( ) );
	++m_numAsteroidsAllocated;
	}
	}
	}
	}
	}
	if ( m_numAsteroidsAllocated == 0 )
	{
	++m_currentWave;
	for ( int i = 0; i < INITIAL_NUMBER_OF_ASTEROIDS * m_currentWave; i++ )
	{
	if ( m_numAsteroidsAllocated < MAX_NUMBER_OF_ASTEROIDS )
	m_asteroids[ i ] = new Asteroid( static_cast<int>( g_theApp->screenWidth ), static_cast<int>( g_theApp->screenHeight ) );
	}
	m_numAsteroidsAllocated = m_currentWave * INITIAL_NUMBER_OF_ASTEROIDS;
	if ( m_numAsteroidsAllocated < MAX_NUMBER_OF_ASTEROIDS )
	{
	m_asteroids[ m_numAsteroidsAllocated ] = new Asteroid( static_cast<int>( g_theApp->screenWidth ), static_cast<int>( g_theApp->screenHeight ) );
	m_asteroids[ m_numAsteroidsAllocated ]->SetSize( m_currentWave + ASTEROID_SIZE_MAX );
	++m_numAsteroidsAllocated;
	}
	}


	for ( int i = 0; i < m_numBulletsAllocated; i++ )
	{
	m_bullets[ i ]->Update( deltaSeconds );
	if ( !m_bullets[ i ]->IsAlive( ) )
	{
	if ( m_bullets[ i ]->GetBulletType( ) != Bullet::STANDARD ) SpawnBulletBurst( m_bullets[ i ]->GetBulletType( ) * Bullet::CLUSTER_BULLET_STORM_SIZE, m_bullets[ i ]->GetPosition( ), m_bullets[ i ]->GetOrientation( ) );
	delete m_bullets[ i ];
	m_bullets[ i ] = nullptr;
	if ( m_numBulletsAllocated > 1 ) //Fill gap in array.
	{
	m_bullets[ i ] = m_bullets[ m_numBulletsAllocated - 1 ];
	m_bullets[ m_numBulletsAllocated - 1 ] = nullptr;
	}
	--m_numBulletsAllocated;
	}
	}


	if ( m_ship->m_readyToExplode )
	{
	m_ship->m_readyToExplode = false;
	//Goal is to spawn bullets in all directions from the position and with the velocity of the ship.SpawnBulletBurst();
	SpawnBulletBurst( NUMBER_OF_BULLETS_SHIP_EXPLODES_INTO_KNOB, m_ship->GetLocalTipPosition( ) + m_ship->GetPosition( ), m_ship->GetOrientation( ) );
	}


	if ( //Controller.
	( g_theApp->controllers[ TheApp::CONTROLLER_ONE ] != nullptr &&
	g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->IsStartButtonDown( ) &&
	g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->WasStartButtonJustPressed( ) )
	|| //Keyboard.
	( g_theApp->isKeyDown( 'P' ) && m_ship->IsAlive( ) == false )
	)
	{
	m_ship->SetPosition( static_cast<float>( g_theApp->screenWidth ) / 2.f, static_cast<float>( g_theApp->screenHeight ) / 2.f );
	m_ship->SetVelocity( 0.f, 0.f );
	m_ship->SetOrientation( PI / 2.f );
	m_ship->SetAngularVelocity( 0.f );
	m_ship->SetIsAlive( true );
	}

	if ( g_theApp->isKeyDown( 'O' ) && g_theApp->WasKeyJustPressed( 'O' ) && m_numAsteroidsAllocated < MAX_NUMBER_OF_ASTEROIDS )
	{
	m_asteroids[ m_numAsteroidsAllocated ] = new Asteroid( );
	++m_numAsteroidsAllocated;
	}

	if ( g_theApp->isKeyDown( 'L' ) && g_theApp->WasKeyJustPressed( 'L' ) && m_numAsteroidsAllocated > 0 ) //Destroy.
	{
	delete m_asteroids[ m_numAsteroidsAllocated - 1 ]; //-1 makes the count 0-based.
	m_asteroids[ m_numAsteroidsAllocated - 1 ] = nullptr;
	--m_numAsteroidsAllocated;
	}

	if ( m_ship->IsAlive( ) == false ) return; //Dead ships don't shoot.

	if ( m_numBulletsAllocated < MAX_NUMBER_OF_BULLETS &&
	( //Controller.
	( g_theApp->controllers[ TheApp::CONTROLLER_ONE ] != nullptr &&
	g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->IsAButtonDown( ) &&
	g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->WasAButtonJustPressed( ) )
	|| //Keyboard.
	( g_theApp->isKeyDown( VK_SPACE ) && g_theApp->WasKeyJustPressed( VK_SPACE ) ) ||
	( g_theApp->isKeyDown( VK_NUMPAD1 ) && g_theApp->WasKeyJustPressed( VK_NUMPAD1 ) ) ||
	( g_theApp->isKeyDown( '1' ) && g_theApp->WasKeyJustPressed( '1' ) )
	)
	)
	{
	m_bullets[ m_numBulletsAllocated ] = new Bullet( m_ship->GetLocalTipPosition( ) + m_ship->GetPosition( ), m_ship->GetOrientation( ) );
	++m_numBulletsAllocated;
	m_numBulletsFired += 1;
	return; //No firing multiple bullet types at once.
	}
	*/
}


//-----------------------------------------------------------------------------
void TheGame::Render( )
{

}
