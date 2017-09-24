
/************************************************************************
*    FILE NAME:       basicspritestrategy2d.h
*
*    DESCRIPTION:     Basic sprite strategy 2d class
************************************************************************/

#ifndef __basic_sprite_strategy_2d_h__
#define __basic_sprite_strategy_2d_h__

// Physical component dependency
#include <common/basestrategy.h>

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <common/worldvalue.h>
#include <common/spritedatacontainer.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <map>

// Forward Declarations
class iSprite2D;
class CSpriteDataContainer;
class CMatrix;

class CBasicSpriteStrategy2D : public CBaseStrategy, boost::noncopyable
{
public:

    // Constructor
    CBasicSpriteStrategy2D( int idOffset = 0, int idDir = 1 );

    // Destructor
    virtual ~CBasicSpriteStrategy2D();
    
    // Load the sprite data from file
    void LoadFromFile( const std::string & file ) override;

    // Create the sprite
    virtual void Create(
        const std::string & name,
        const int id,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ) override;

    // Update the sprites
    void Update() override;

    // Transform the sprite
    void Transform() override;

    // Render the sprites
    void Render( const CMatrix & matrix ) override;
    
    // Get the reference to the sprite
    template<typename target>
    target & Get( const int id )
    {
        return *dynamic_cast<target *>(get( id ));
    }
    
    // Get the sprite data by name
    CSpriteDataContainer & GetData( const std::string & name );

protected:
    
    // Handle the deleting of any sprites
    void DeleteObj( int index ) override;
    
    // Handle the creating of any object by name
    void CreateObj( const std::string & name ) override;
    
    // Get the pointer to the sprite
    iSprite2D * get( const int id );

protected:
    
    // Map of the sprite data
    std::map<const std::string, CSpriteDataContainer> m_dataMap;

    // Map of all the sprites
    std::map<const int, iSprite2D *> m_spriteMap;
    
    // iterator for easy access to the last created sprite
    std::pair<std::map<const int, iSprite2D *>::iterator, bool> m_iter;
};

#endif  // __basic_sprite_strategy_2d_h__

