/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by Andr� Schnabel (thefrogs@web.de)

*******************************************************************************/

// Enemy.h

#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.h"

#define ENEMY_SHIFT     300

// Any enemy should derive from this class!
class CEnemy : public CActor
{
    public:
        CEnemy( int s_x, int s_y,
			SLevel *level,
                SDL_Texture *images[NUM_ACTOR_IMAGES],
                int fall_speed, bool shotable );        
        
        virtual void Update( int *scroll_x, int *scroll_y ) = 0; // Behavior
        
        int GetScoreValue() { return m_score_value; }
        void SetScoreValue( int score_value ) { m_score_value = score_value; }
        
        int GetId() { return m_id; }
        void SetId( int id ) { m_id = id; }
        
        bool IsShootable() { return m_shootable; }
        void SetShootable( bool shootable ) { m_shootable = shootable; }
        
    private:
        bool m_shootable;
        int m_id;
        int m_score_value;
};

class CWalkingEnemy : public CEnemy
{
    public:
        CWalkingEnemy(	int s_x, int s_y,
						SLevel *level,
						SDL_Texture *images[NUM_ACTOR_IMAGES],
						int fall_speed, int walk_speed );
        
        virtual void Update( int *scroll_x, int *scroll_y );
    private:
        int m_walk_speed;
        Direction m_walking_dir;
};


// Blob

#define ENEMY_BLOB      0
#define BLOB_FALL_SPEED 9
#define BLOB_WALK_SPEED 2
#define BLOB_SCORE      10

class CBlob : public CWalkingEnemy
{
    public:
        CBlob(	int s_x, int s_y,
                SLevel *level,
                SDL_Texture *images[NUM_ACTOR_IMAGES], int walk_speed );
};

// Robot

#define ENEMY_ROBOT         1
#define ROBOT_WALK_SPEED    3
#define ROBOT_FALL_SPEED    11
#define ROBOT_SCORE         20

class CRobot : public CWalkingEnemy
{
    public:
        CRobot( int s_x, int s_y,
                SLevel *level,
                SDL_Texture *images[NUM_ACTOR_IMAGES], int walk_speed );
};

// Stoney

#define ENEMY_STONEY        2
#define STONEY_SCORE        0
#define STONEY_JUMP_INCREASE 20
#define STONEY_FALL_SPEED   5

class CStoney : public CEnemy
{
    public:
        CStoney( int s_x, int s_y,
                 SLevel *level,
                 SDL_Texture *images[NUM_ACTOR_IMAGES] );
    
        virtual void Update( int *scroll_x, int *scroll_y );
};

// Plant

#define ENEMY_PLANT         3
#define PLANT_SCORE         0
//#define PLANT_JUMP_INCREASE 2 ??

class CPlant : public CEnemy
{
    public:
        CPlant( int s_x, int s_y,
                SLevel *level,
                SDL_Texture *images[NUM_ACTOR_IMAGES] );
                
        virtual void Update( int *scroll_x, int *scroll_y );
    private:
        int m_count;
        Uint32 m_last_time;
};

// Snail

#define ENEMY_SNAIL			4
#define SNAIL_WALK_SPEED	1
#define SNAIL_FALL_SPEED	7
#define SNAIL_SCORE			20

class CSnail : public CWalkingEnemy
{
	public:
		CSnail( int s_x, int s_y,
                SLevel *level,
                SDL_Texture *images[NUM_ACTOR_IMAGES], int walk_speed );
};

// Rocketball

#define ENEMY_ROCKETBALL		5
#define ROCKETBALL_WALK_SPEED	4
#define ROCKETBALL_FALL_SPEED	12
#define ROCKETBALL_SCORE		25

class CRocketBall : public CWalkingEnemy
{
	public:
		CRocketBall(	int s_x, int s_y,
						SLevel *level,
						SDL_Texture *images[NUM_ACTOR_IMAGES], int walk_speed );
};

#endif
