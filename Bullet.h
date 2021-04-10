/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// Bullet.h

#ifndef BULLET_H
#define BULLET_H

#include "Framework.h"
#include "Sprite.h"
#include "SLevel.h"
#include "Enemy.h"

class CBullet : public CSprite
{
    public:
        CBullet(	SDL_Texture *surf,
					int x, int y, int speed,
					Direction dir, SLevel *level,
					CLinkedList< CEnemy > *enemies )
        {
			SetImage( surf );

            m_speed = speed;
            m_dir = dir;
            m_level = level;
            m_enemies = enemies;
            m_victim = NULL,
            m_killed_enemy = false;
            
            m_rect.x = x;
            m_rect.y = y;

            int w, h;
            SDL_QueryTexture(surf, nullptr, nullptr, &w, &h);

            m_rect.w = w;
            m_rect.h = h;
        }
        
        // Returns false if this bullet must be deleted
        bool Update( int *scroll_x, int *scroll_y )
        {
            int i, j;
            
            // Approximation
            for( i=0; i<m_speed; i++ )
            {
                // Fly left
                if( m_dir == DIR_LEFT )
                {
                    if( m_rect.x-1 > 0 )
                        m_rect.x-=1;
                }
                // Fly right
                else
                {
                    if( m_rect.x+1 < LVL_W_MAX * TILE_W )
                        m_rect.x+=1;
                }
                
                // Collision with tile?
                if( m_level->content[m_rect.y/TILE_H][m_rect.x/TILE_W] != TILE_EMPTY )
                    return false;
                
                // Collision with enemy
                for( j=0; j<m_enemies->GetNumberOfElements(); j++ )
                {
                    if( g_framework->Coll( m_enemies->GetData( j )->GetRect(), &m_rect ) )
                    {
                        if( !m_enemies->GetData( j )->GetDying() && m_enemies->GetData( j )->IsShootable() )
                        {
                            m_enemies->GetData( j )->Die();
                            m_killed_enemy = true;
                            m_victim = m_enemies->GetData( j );
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        
        bool EnemyKilled() { return m_killed_enemy; }
        CEnemy *GetVictim() { return m_victim; }
        
    private:
        bool m_killed_enemy;
        CEnemy *m_victim;
        Direction m_dir;
        int m_speed;
        SLevel *m_level;
        CLinkedList< CEnemy > *m_enemies;
};

#endif
