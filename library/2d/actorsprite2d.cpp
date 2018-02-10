
/************************************************************************
*    FILE NAME:       actorsprite2d.cpp
*
*    DESCRIPTION:     2D Actor Sprite Class
************************************************************************/

// Physical component dependency
#include <2d/actorsprite2d.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <common/actordata.h>
#include <common/size.h>
#include <2d/iaibase2d.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>

// SDL/OpenGL lib dependencies
#include <SDL.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CActorSprite2D::CActorSprite2D( const CActorData & actorData, int id ) :
    m_radius(0),
    m_scaledRadius(0),
    m_projectionType(CSettings::Instance().GetProjectionType()),
    m_id(id),
    m_collisionGroup(0),
    m_collisionMask(0)
{
    // Create the actor based on the data
    Create( actorData );
    
    // Set the sprite type
    m_parameters.Add( NDefs::ACTOR2D );

}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CActorSprite2D::~CActorSprite2D()
{
}   // destructor


/************************************************************************
*    desc:  Set/Get the AI pointer. This class owns the pointer
************************************************************************/
void CActorSprite2D::SetAI( iAIBase2D * pAIBase )
{
    m_upAI.reset(pAIBase);

    // Handle any initialization in a separate function
    m_upAI->Init();

}   // SetAI


/************************************************************************
*    desc:  Create the actor's sprites
************************************************************************/
void CActorSprite2D::Create( const CActorData & actorData )
{
    const auto & spriteDataVec = actorData.GetSpriteData();

    CSize<float> largestSize;

    for( auto & iter: spriteDataVec )
    {
        // Allocate the sprite and add it to the map for easy access
        m_spriteDeq.emplace_back( CObjectDataMgr::Instance().GetData2D( iter ), iter );
        
        // If there's a name for this sprite, add it to the map
        if( !iter.GetName().empty() )
        {
            auto duplicateCheckIter = m_pSpriteMap.emplace( iter.GetName(), &m_spriteDeq.back() );
            
            // Check for duplicate names
            if( !duplicateCheckIter.second )
            {
                throw NExcept::CCriticalException("Actor Sprite Creation !rror!",
                    boost::str( boost::format("Duplicate sprite name (%s).\n\n%s\nLine: %s")
                        % iter.GetName() % __FUNCTION__ % __LINE__ ));
            }
        }

        // Find the largest size width and height of the different sprites for the controls size
        const CSize<float> & size = m_spriteDeq.back().GetObjectData().GetSize();
        const CPoint<CWorldValue> & pos = m_spriteDeq.back().GetPos();
        const CPoint<float> & scale = m_spriteDeq.back().GetScale();

        const float width( (size.w + std::fabs(pos.x)) * scale.x );
        const float height( (size.h + std::fabs(pos.y)) * scale.y );

        // Find the largest size to use as radius
        if( width > largestSize.w )
            largestSize.w = width;

        if( height > largestSize.h )
            largestSize.h = height;
    }
    
    // Copy over the transform for the Actor sprite
    CopyTransform( &actorData );

    // Convert the largest width and height to a radius
    largestSize /= 2;
    m_radius = largestSize.GetLength();
    m_scaledRadius = m_radius;
    
    // Set the collision filter info
    m_collisionGroup = actorData.GetCollisionGroup();
    m_collisionMask = actorData.GetCollisionMask();
    m_collisionRadiusScalar = actorData.GetCollisionRadiusScalar();
    
    // Init the radius for collision
    m_collisionRadius = m_scaledRadius * m_collisionRadiusScalar;

}   // Create


/************************************************************************
*    desc:  Init the physics                                                           
************************************************************************/
void CActorSprite2D::InitPhysics()
{

}


/************************************************************************
*    desc:  React to what the player is doing
************************************************************************/
void CActorSprite2D::HandleEvent( const SDL_Event & rEvent )
{
    if( m_upAI )
        m_upAI->HandleEvent( rEvent );

}   // HandleEvent


/************************************************************************
*    desc:  Update the actor
************************************************************************/
void CActorSprite2D::Update()
{
    if( m_upAI )
        m_upAI->Update();

    for( auto & iter : m_spriteDeq )
        iter.Update();

}   // Update


/************************************************************************
*    desc:  Update the physics
************************************************************************/
void CActorSprite2D::PhysicsUpdate()
{

}   // PhysicsUpdate


/************************************************************************
*    desc:  Transform the actor
************************************************************************/
void CActorSprite2D::Transform()
{
    CObject2D::Transform();

    for( auto & iter : m_spriteDeq )
        iter.Transform( GetMatrix(), WasWorldPosTranformed() );

}   // Transform

void CActorSprite2D::Transform( const CObject2D & object )
{
    CObject2D::Transform( object.GetMatrix(), object.WasWorldPosTranformed() );

    for( auto & iter : m_spriteDeq )
        iter.Transform( GetMatrix(), WasWorldPosTranformed() );

}   // Transform

void CActorSprite2D::Transform( const CMatrix & matrix, bool tranformWorldPos )
{
    CObject2D::Transform( matrix, tranformWorldPos );

    for( auto & iter : m_spriteDeq )
        iter.Transform( GetMatrix(), WasWorldPosTranformed() );

}   // Transform


/************************************************************************
*    desc:  Render the actor
************************************************************************/
void CActorSprite2D::Render( const CMatrix & matrix )
{
    // Render in reverse order
    if( InView() )
    {
        for( auto & iter : m_spriteDeq )
            iter.Render( matrix );
    }

}   // Render


/************************************************************************
*    desc:  Get the physics component                                                            
************************************************************************/
CPhysicsComponent2D & CActorSprite2D::GetPhysicsComponent()
{
    return m_physicsComponent;
    
}   // GetPhysicsComponent


/************************************************************************
*    desc:  Get the sprite
************************************************************************/
CSprite2D & CActorSprite2D::GetSprite( int index )
{
    if( index < 0 )
        return m_spriteDeq.back();
    else
        return m_spriteDeq.at(index);

}   // GetSprite


/************************************************************************
*    desc:  Get the sprite group
************************************************************************/
CSprite2D & CActorSprite2D::GetSprite( const std::string & name )
{
    auto iter = m_pSpriteMap.find( name );
    if( iter == m_pSpriteMap.end() )
        throw NExcept::CCriticalException( "Actor Sprite Access Error!",
            boost::str( boost::format("Sprite name does not exist (%s).\n\n%s\nLine: %s")
                % name % __FUNCTION__ % __LINE__ ) );

    return *iter->second;

}   // GetSprite


/************************************************************************
*    desc:  Render the actor
************************************************************************/
bool CActorSprite2D::InView()
{
    if( m_projectionType == NDefs::EPT_ORTHOGRAPHIC )
        return InOrthographicView();

    else if( m_projectionType == NDefs::EPT_PERSPECTIVE )
        return InPerspectiveView();

    return true;

}   // InView


/************************************************************************
 *    desc:  Check if an object is within the orthographic view frustum
 ************************************************************************/
bool CActorSprite2D::InOrthographicView()
{
    const CSize<float> & defaultSizeHalf = CSettings::Instance().GetDefaultSizeHalf();
    
    // Check against the right side of the screen
    if( std::fabs(m_transPos.x) > (defaultSizeHalf.w + m_scaledRadius) )
        return false;

    // Check against the top of the screen
    if( std::fabs(m_transPos.y) > (defaultSizeHalf.h + m_scaledRadius) )
        return false;

    // If we made it this far, the object is in view
    return true;

}   // InOrthographicView


/************************************************************************
 *    desc:  Check if an object is within the perspective view frustum
 ************************************************************************/
bool CActorSprite2D::InPerspectiveView()
{
    const CSize<float> & aspectRatio = CSettings::Instance().GetScreenAspectRatio();

    // Check the right and left sides of the screen
    if( std::fabs(m_transPos.x) > ((std::fabs(m_pos.z) * aspectRatio.w) + m_scaledRadius) )
        return false;

    // Check the top and bottom sides of the screen
    if( std::fabs(m_transPos.y) > ((std::fabs(m_pos.z) * aspectRatio.h) + m_scaledRadius) )
        return false;

    // if we made it this far, the object was not culled
    return true;

}   // InPerspectiveView


/************************************************************************
*    desc:  Get the unique id number
************************************************************************/
int CActorSprite2D::GetId() const
{
    return m_id;

}   // GetId


/************************************************************************
*    desc:  Apply the scale to the radius
************************************************************************/
void CActorSprite2D::ApplyScale( CMatrix & matrix )
{
    CObject2D::ApplyScale( matrix );

    // Find the largest actor scale
    float scale = GetScale().x;
    if( GetScale().y > scale )
        scale = GetScale().y;

    m_scaledRadius = m_radius * scale;
    
    // Init the radius for collision
    m_collisionRadius = m_scaledRadius * m_collisionRadiusScalar;

}   // ApplyScale


/************************************************************************
*    desc:  Get the collision group
************************************************************************/
uint CActorSprite2D::GetCollisionGroup() const
{
    return m_collisionGroup;
}


/************************************************************************
*    desc:  Get the collision radius
************************************************************************/
float CActorSprite2D::GetCollisionRadius() const
{
    return m_collisionRadius;
}


/***************************************************************************
*    desc:  Check for broad phase collision against other actor sprite
****************************************************************************/
bool CActorSprite2D::IsCollision( CActorSprite2D & rActor )
{
    bool result(false);
    
    if( m_collisionMask & rActor.GetCollisionGroup() )
    {
        result = CheckBroadPhase( rActor );
    }
    
    return result;
    
}   // IsCollision


/***************************************************************************
*    desc:  Check for broad phase collision against other actor sprite
****************************************************************************/
bool CActorSprite2D::CheckBroadPhase( CActorSprite2D & rActor )
{
    const float radius = m_collisionRadius + rActor.GetCollisionRadius();
    const float length = m_transPos.GetLength2D( rActor.GetTransPos() );
    
    return (length < radius);
    
}   // CheckBroadPhase
