/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Enemy.cpp

#include "Enemy.h"

//
// CEnemy
// Constructor
//
CEnemy::CEnemy( int s_x, int s_y, SLevel *level,
                SDL_Texture *images[NUM_ACTOR_IMAGES], int fall_speed, bool shootable )
: CActor( s_x, s_y, level, images, fall_speed )
{
    m_shootable = shootable;
    
    // Reset score
    SetScoreValue( 0 );
}

//
// CWalkingEnemy
// Constructor
//
CWalkingEnemy::CWalkingEnemy(	int s_x, int s_y, SLevel *level,
								SDL_Texture *images[NUM_ACTOR_IMAGES], int fall_speed, int walk_speed )
: CEnemy( s_x, s_y, level, images, fall_speed, true )
{
    // Start to walk into the right direction right now
    if( WalkPossible( DIR_LEFT ) )
        m_walking_dir = DIR_LEFT;
    else
        m_walking_dir = DIR_RIGHT;
        
    m_walk_speed = walk_speed;
}

//
// Update
//
void CWalkingEnemy::Update( int *scroll_x, int *scroll_y )
{
    // Walk until blob hits an obstacle and then reverse the direction
    if( WalkPossible( m_walking_dir ) )
        Walk( m_walking_dir, m_walk_speed );    
    else
        g_framework->ReverseDirection( &m_walking_dir );        
    
    // Standard update func
    CActor::Update( scroll_x, scroll_y, true );
}

//
// CBlob
// Constructor
//
CBlob::CBlob(	int s_x, int s_y, SLevel *level,
				SDL_Texture *images[NUM_ACTOR_IMAGES], int walk_speed )
: CWalkingEnemy( s_x, s_y, level, images, BLOB_FALL_SPEED, walk_speed )
{    
    SetId( ENEMY_BLOB );
    // Blob specific score
    SetScoreValue( BLOB_SCORE );
}

//
// CRobot
// Constructor
//
CRobot::CRobot( int s_x, int s_y, SLevel *level,
				SDL_Texture *images[NUM_ACTOR_IMAGES], int walk_speed )
: CWalkingEnemy( s_x, s_y, level, images, ROBOT_FALL_SPEED, walk_speed )
{    
    SetId( ENEMY_ROBOT );
    // Robot specific score
    SetScoreValue( ROBOT_SCORE );
}

//
// CStoney
// Constructor
//
CStoney::CStoney(	int s_x, int s_y, SLevel *level,
					SDL_Texture *images[NUM_ACTOR_IMAGES] )
: CEnemy( s_x, s_y, level, images, STONEY_FALL_SPEED, false )
{
    SetId( ENEMY_STONEY );
    SetScoreValue( STONEY_SCORE );
    
    // Because normally you come from left
    SetDirection( DIR_LEFT );
}

//
// Update
//
void CStoney::Update( int *scroll_x, int *scroll_y )
{
    int i;
    
    // Standard update func
    CActor::Update( scroll_x, scroll_y, true );
    
    if( !GetFalling() && !IsJumping() )
    {
        Jump();
        for( i=0; i<STONEY_JUMP_INCREASE; i+=JUMP_INCREASE_POWER )
            IncreaseJump( true );
    }
}

//
// CPlant
// Constructor
//
CPlant::CPlant(	int s_x, int s_y, SLevel *level,
				SDL_Texture *images[NUM_ACTOR_IMAGES] )
: CEnemy( s_x, s_y, level, images, 0, false )
{
    SetId( ENEMY_PLANT );
    SetScoreValue( PLANT_SCORE );
    
    SetImage( m_images[DEAD_RIGHT] );
    // This is stupid...
    SetDirection( DIR_RIGHT );
    
    m_last_time = g_framework->GetTicks();
    m_count = 0;
}

//
// Update
//
void CPlant::Update( int *scroll_x, int *scroll_y )
{
    if( g_framework->GetTicks() - m_last_time > 1000 )
    {
        m_last_time = g_framework->GetTicks();
        if( m_count < NUM_ACTOR_IMAGES/2-1 )
            m_count++;
        else
            m_count = 1;
        SetImage( m_images[m_count] );
    }
    
    // Standard update func
    CActor::Update( scroll_x, scroll_y, true );
}

//
// CSnail
// Constructor
//
CSnail::CSnail( int s_x, int s_y, SLevel *level,
				SDL_Texture *images[NUM_ACTOR_IMAGES], int walk_speed )
: CWalkingEnemy( s_x, s_y, level, images, SNAIL_FALL_SPEED, walk_speed )
{
	SetId( ENEMY_SNAIL );
	// Snail specific score
	SetScoreValue( SNAIL_SCORE );
}

//
// CRocketBall
// Constructor
//
CRocketBall::CRocketBall(	int s_x, int s_y, SLevel *level,
							SDL_Texture *images[NUM_ACTOR_IMAGES], int walk_speed )
: CWalkingEnemy( s_x, s_y, level, images, ROCKETBALL_FALL_SPEED, walk_speed )
{
	SetId( ENEMY_ROCKETBALL );
	// Rocketball specific score
	SetScoreValue( ROCKETBALL_SCORE );
}