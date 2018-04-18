
/************************************************************************
*    FILE NAME:       basicspritestrategy3d.h
*
*    DESCRIPTION:     Basic sprite strategy 3d class
************************************************************************/

#ifndef __basic_sprite_strategy_3d_h__
#define __basic_sprite_strategy_3d_h__

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
class iSprite3D;
class CSpriteDataContainer;
class CMatrix;

class CBasicSpriteStrategy3D : public CBaseStrategy, boost::noncopyable
{
public:

    // Constructor
    CBasicSpriteStrategy3D( const std::string & cameraId = "", int idOffset = 0, int idDir = 1 );

    // Destructor
    virtual ~CBasicSpriteStrategy3D();
    
    // Load the sprite data from file
    void LoadFromFile( const std::string & file ) override;

    // Create the sprite
    virtual int Create(
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
    void Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix ) override;
    
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
    iSprite3D * get( const int id );

protected:
    
    // Map of the sprite data
    std::map<const std::string, CSpriteDataContainer> m_dataMap;

    // Map of all the sprites
    std::map<const int, iSprite3D *> m_spriteMap;
    
    // iterator for easy access to the last created sprite
    std::pair<std::map<const int, iSprite3D *>::iterator, bool> m_iter;
};

#endif  // __basic_sprite_strategy_3d_h__


