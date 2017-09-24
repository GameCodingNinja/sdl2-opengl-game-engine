
/************************************************************************
*    FILE NAME:       actordata.h
*
*    DESCRIPTION:     Actor Data Class
************************************************************************/

#ifndef __actor_data_h__
#define __actor_data_h__

// Physical component dependency
#include <2d/object2d.h>

// Game lib dependencies
#include <common/spritedata.h>
#include <common/defs.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward Declarations
struct XMLNode;

class CActorData : public CObject
{
public:

    // Constructor
    CActorData(
        const XMLNode & node,
        const std::string & defGroup,
        const std::string & defObjName,
        const std::string & defAIName,
        int defId = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CActorData();

    // Get the AI name
    const std::string & GetAIName() const;
    
    // Get sprite Data
    const std::vector<CSpriteData> & GetSpriteData() const;
    
    // Is this actor a player
    bool IsPlayerActor() const;
    
    // Get the collision group
    uint GetCollisionGroup() const;
    
    // Get the collision Mask
    uint GetCollisionMask() const;
    
    // Get the collision Radius Scalar
    float GetCollisionRadiusScalar() const;
    
    // Get the unique id number
    int GetId() const;

private:

    // Name of the ai
    std::string m_aiName;
    
    // Flag to indicate this is the player actor
    bool m_playerActor;

    // sprite data vector
    std::vector<CSpriteData> m_spriteDataVec;
    
    // Collision group
    uint m_collisionGroup;
    
    // Collision mask
    uint m_collisionMask;
    
    // Scalar to apply to the radius
    float m_collisionRadiusScalar;
    
    // Unique Id number
    int m_id;
};

#endif  // __actor_data_h__