
/************************************************************************
*    FILE NAME:       spritedatacontainer.h
*
*    DESCRIPTION:     Sprite Data Container Class
************************************************************************/

#ifndef __sprite_data_container_h__
#define __sprite_data_container_h__

// Game lib dependencies
#include <common/defs.h>

// Forward Declarations
class CSpriteData;
class CActorData;

class CSpriteDataContainer
{
public:

    // constructor
    CSpriteDataContainer( CSpriteData * pData, int type );
    CSpriteDataContainer( CActorData * pData, int type );

    // destructor
    ~CSpriteDataContainer();
    
    // Get the data type
    int GetType() const;
    
    // Get the data
    template<typename T>
    T & Get() const;
    
private:

    // Sprite Type
    int m_type = NDefs::NONE;

    // Pointer to sprite data class. We own the pointer
    union USpriteDataPtr
    {
        USpriteDataPtr( CSpriteData * pData ) : m_pSpriteData(pData)
        {}
        USpriteDataPtr( CActorData * pData ) : m_pActorData(pData)
        {}

        CSpriteData * m_pSpriteData;
        CActorData * m_pActorData;

    } m_data;
};

template<> inline CSpriteData & CSpriteDataContainer::Get() const { return *m_data.m_pSpriteData; }
template<> inline CActorData  & CSpriteDataContainer::Get() const { return *m_data.m_pActorData; }

#endif  // __sprite_data_h__


