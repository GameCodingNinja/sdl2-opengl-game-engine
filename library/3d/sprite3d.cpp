
/************************************************************************
 *    FILE NAME:       sprite3d.cpp
 *
 *    DESCRIPTION:     3D sprite class
 ************************************************************************/

// Physical component dependency
#include <3d/sprite3d.h>

// Game lib dependencies
#include <objectdata/objectdata3d.h>
#include <objectdata/objectphysicsdata3d.h>
#include <utilities/exceptionhandling.h>
#include <common/color.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

/************************************************************************
 *    desc:  Constructor
 ************************************************************************/
CSprite3D::CSprite3D( const CObjectData3D & objectData ) :
    m_objectData( objectData ),
    m_visualComponent( objectData.GetVisualData() ),
    m_physicsComponent( objectData.GetPhysicsData() )
{
    // If there's no visual data, set the hide flag
    SetVisible( objectData.GetVisualData().IsActive() );

}   // constructor


/************************************************************************
 *    desc:  destructor                                                             
 ************************************************************************/
CSprite3D::~CSprite3D()
{
}   // destructor


/************************************************************************
 *    desc:  Init the physics                                                           
 ************************************************************************/
void CSprite3D::InitPhysics()
{
    m_physicsComponent.Init( *this );
}


/************************************************************************
 *    desc:  Update the sprite                                                           
 ************************************************************************/
void CSprite3D::Update()
{
    if( IsVisible() )
        m_physicsComponent.Update( this );

    m_scriptComponent.Update();

}   // Update


/************************************************************************
 *    desc:  do the render                                                            
 ************************************************************************/
void CSprite3D::Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix )
{
    if( IsVisible() )
        m_visualComponent.Render( m_matrix * projMatrix, m_rotMatrix * cameraMatrix );

}   // Render


/************************************************************************
 *    desc:  Get the visual component                                                            
 ************************************************************************/
CVisualComponent3D & CSprite3D::GetVisualComponent()
{
    return m_visualComponent;

}   // GetVisualComponent


/************************************************************************
 *    desc:  Get the physics component                                                            
 ************************************************************************/
CPhysicsComponent3D & CSprite3D::GetPhysicsComponent()
{
    return m_physicsComponent;

}   // GetPhysicsComponent


/************************************************************************
 *    desc:  Get the scripting component                                                            
 ************************************************************************/
CScriptComponent & CSprite3D::GetScriptComponent()
{
    return m_scriptComponent;

}   // GetScriptingComponent


/************************************************************************
 *    desc:  Get the object data                                                            
 ************************************************************************/
const CObjectData3D & CSprite3D::GetObjectData() const
{
    return m_objectData;

}   // GetObjectData


/************************************************************************
 *    desc:  Set the color
 ************************************************************************/
void CSprite3D::SetColor( const CColor & color )
{
    m_visualComponent.SetColor( color );

}   // SetColor

void CSprite3D::SetRGBA( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_visualComponent.SetRGBA( r, g, b, a );

}   // SetRGBA


/************************************************************************
 *    desc:  Set the default color
 ************************************************************************/
void CSprite3D::SetDefaultColor()
{
    m_visualComponent.SetColor( m_objectData.GetVisualData().GetColor() );

}   // SetColor


/************************************************************************
 *    desc:  Get the color
 ************************************************************************/
const CColor & CSprite3D::GetColor() const
{
    return m_visualComponent.GetColor();

}   // GetColor


/************************************************************************
 *    desc:  Get the default color
 ************************************************************************/
const CColor & CSprite3D::GetDefaultColor() const
{
    return m_objectData.GetVisualData().GetColor();

}   // GetDefaultColor


/************************************************************************
 *    desc:  Set the Alpha
 ************************************************************************/
void CSprite3D::SetAlpha( float alpha )
{
    if( alpha > 1.5 )
        alpha *= defs_RGB_TO_DEC;

    m_visualComponent.SetAlpha( alpha );

}   // SetAlpha


/************************************************************************
 *    desc:  Get the Alpha
 ************************************************************************/
float CSprite3D::GetAlpha() const
{
    return m_visualComponent.GetAlpha();

}   // GetAlpha


/************************************************************************
 *    desc:  Get the default alpha
 ************************************************************************/
float CSprite3D::GetDefaultAlpha() const
{
    return m_objectData.GetVisualData().GetColor().GetA();

}   // GetDefaultAlpha


/************************************************************************
 *    desc:  Register the class with AngelScript
 ************************************************************************/
void CSprite3D::Register( asIScriptEngine * pEngine )
{
    using namespace NScriptGlobals;
    
    // Register CScriptComponent2d reference and methods
    Throw( pEngine->RegisterObjectType(  "CSprite3d", 0, asOBJ_REF | asOBJ_NOCOUNT) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetVisible(bool visible)",                    asMETHOD(CObject,   SetVisible),         asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetColor(const CColor & in)",                 asMETHOD(CSprite3D, SetColor),           asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetRGBA(float r, float g, float b, float a)", asMETHOD(CSprite3D, SetRGBA),            asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetDefaultColor()",                           asMETHOD(CSprite3D, SetDefaultColor),    asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "const CColor & GetColor()",                        asMETHOD(CSprite3D, GetColor),           asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "const CColor & GetDefaultColor()",                 asMETHOD(CSprite3D, GetDefaultColor),    asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetAlpha(float alpha)",                       asMETHOD(CSprite3D, SetAlpha),           asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "float GetAlpha()",                                 asMETHOD(CSprite3D, GetAlpha),           asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "float GetDefaultAlpha()",                          asMETHOD(CSprite3D, GetDefaultAlpha),    asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "CPoint GetPos()",                                  asMETHOD(CObject,   GetPos_AngelScript), asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetPos(CPoint & in)",                         asMETHOD(CObject,   SetPos_AngelScript), asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite3d", "void IncRot(CPoint & in)",                         asMETHOD(CObject,   IncRot),             asCALL_THISCALL) );

}   // Register