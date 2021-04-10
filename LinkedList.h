/*******************************************************************************

    CommanderTux
    Penguin In Space

    Released under the GNU Public License    
    2005 by André Schnabel (thefrogs@web.de)

*******************************************************************************/

// LinkedList.h
// Mostly needed for enemies and items in the game

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

template < class Type >
class CLinkedList
{
    public:
        
        // Element
        struct SElement {
            Type *data;
            SElement *next, *prev;
            
            SElement( Type *ndata ) {
                next = prev = NULL;
                data = ndata;
            }
            ~SElement() {
                if( data )
                    delete data;
                if( next )
                    next->prev = prev;
                if( prev )
                    prev->next = next;
            }
        };
        
        // Constructor
        CLinkedList()
        {
            m_head = m_tail = NULL;
            m_selected = NULL;
            m_number_of_elements = 0;
        }
        
        // Destructor
        ~CLinkedList() { Empty(); }
        
        // Add new element to the list
        void Add( Type *data )
        {
            SElement *new_element;
            
            if( !data )
                return;
            
            new_element = new SElement( data );
            
            if( !m_head ) // First element
            {
                m_head = new_element;
                m_tail = new_element;
            }
            else // Something later
            {
                m_tail->next = new_element;
                new_element->prev = m_tail;
                m_tail = new_element;
            }
            
            m_number_of_elements++;
        }
        
        // Get specific element
        SElement *GetElement( int nr )
        {
            int i;
            if( nr < GetNumberOfElements() )
            {
                m_selected = m_head;
                for( i=0; i<nr; i++ )
                {
                    if( m_selected->next )
                        m_selected = m_selected->next;
                }
                return m_selected;
            }
            else
                return NULL;
        }
        
        // Get data of specific element
        Type *GetData( int element_nr )
        {
            return GetElement( element_nr )->data;
        }
        
        // Delete specific element
        void Delete( int nr )
        {            
            if( GetNumberOfElements() > 0 )
            {
                if( nr == 0 ) // Head
                {
                    m_selected = m_head->next;
                    delete m_head;
                    m_head = m_selected;
                }
                else if( nr < GetNumberOfElements() - 1 )
                {
                    delete GetElement( nr );                    
                }
                else // Tail
                {
                    m_selected = m_tail->prev;
                    delete m_tail;
                    m_tail = m_selected;
                }   
                
                m_number_of_elements--;
            }
        }
        
        // Empty whole list
        void Empty()
        {
            int i;
            for( i=0; i<m_number_of_elements; i++ )
                Delete( i );
                
            m_head = NULL;
            m_tail = NULL;
        }
        
        // Get the head
        SElement *GetHead() { return m_head; }
        
        // And the tail
        SElement *GetTail() { return m_tail; }
        
        // Size of the list?
        int GetNumberOfElements() { return m_number_of_elements; }
        
    private:
        
        SElement *m_head, *m_tail;
        SElement *m_selected;
        int m_number_of_elements;
};

#endif
