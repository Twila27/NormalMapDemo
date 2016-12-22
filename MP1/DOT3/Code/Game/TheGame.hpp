#pragma once


//-----------------------------------------------------------------------------
class TheGame;

//-----------------------------------------------------------------------------
//Add other forward declarations for classes whose objects tracked below:
//class Bullet; //For Bullet* m_bullets[#] below, etc.

//-----------------------------------------------------------------------------
//Add constants required by header file, e.g. sizes of arrays on stack memory.
//const int MAX_NUMBER_OF_BULLETS = 400; //For Bullet* m_bullets[MNOB], etc.

//-----------------------------------------------------------------------------
extern TheGame* g_theGame;


//-----------------------------------------------------------------------------
class TheGame
{
private:
	//int m_numBulletsAllocated; //Check vs to not exceed array size max/min.
public:
	//Ship* m_ship; //Whatever represents the player, array if multiplayer.
	//Bullet* m_bullets[ MAX_NUMBER_OF_BULLETS ];
	TheGame( );
	~TheGame( );
	void Update( float deltaSeconds );
	void Render( );
};
