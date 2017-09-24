
/************************************************************************
*    FILE NAME:       sprite2d.cpp
*
*    DESCRIPTION:     2D sprite class
************************************************************************/

// Physical component dependency
#include <2d/sprite2d.h>

// Game lib dependencies
#include <objectdata/objectdata2d.h>
#include <objectdata/objectphysicsdata2d.h>
#include <utilities/exceptionhandling.h>
#include <common/color.h>
#include <utilities/xmlParser.h>
#include <script/scriptglobals.h>
#include <script/scriptmanager.h>
#include <2d/iaibase2d.h>

// AngelScript lib dependencies
#include <angelscript.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSprite2D::CSprite2D( const CObjectData2D & objectData, int id ) :
    m_rObjectData( objectData ),
    m_visualComponent( objectData.GetVisualData() ),
    m_physicsComponent( objectData.GetPhysicsData() ),
    m_id(id)
{
    // If there's no visual data, set the hide flag
    SetVisible( objectData.GetVisualData().IsActive() );
    
    // Set the sprite type
    m_parameters.Add( NDefs::SPRITE2D );
    
    if( objectData.GetVisualData().GetGenerationType() == NDefs::EGT_SPRITE_SHEET )
        SetCropOffset( objectData.GetVisualData().GetSpriteSheet().GetGlyph().GetCropOffset() );

}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CSprite2D::~CSprite2D()
{
}   // destructor


/************************************************************************
*    desc:  Init the script functions and add them to the map
*           This function loads the attribute info reguardless of what it is
************************************************************************/
void CSprite2D::InitScriptFunctions( const XMLNode & node )
{
    // Check for scripting - Add an empty string for scripts not defined
    XMLNode scriptLstNode = node.getChildNode( "scriptLst" );
    if( !scriptLstNode.isEmpty() )
    {
        for( int i = 0; i < scriptLstNode.nChildNode(); ++i )
        {
            const XMLNode scriptNode = scriptLstNode.getChildNode(i);
            
            // Only the first attribute is used
            const XMLAttribute attribute = scriptNode.getAttribute(0);
            const std::string attrName = attribute.lpszName;
            const std::string attrValue = attribute.lpszValue;

            // Add the attribute name and value to the map
            if( !attrValue.empty() )
                m_scriptFunctionMap.emplace( attrName, attrValue );
        }
    }
    
}   // InitScriptFunctions


/************************************************************************
*    desc:  Prepare the script function to run
************************************************************************/
void CSprite2D::PrepareFuncId( const std::string & scriptFuncId, bool forceUpdate )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        m_scriptComponent.Prepare( m_rObjectData.GetGroup(), iter->second, {this});
        
        // Allow the script to execute and return it's context to the queue
        // for the scripts that don't animate
        if( forceUpdate )
            m_scriptComponent.Update();
    }

}   // Prepare

void CSprite2D::Prepare(
    const std::string & funcName,
    const std::vector<CScriptParam> & paramVec,
    bool forceUpdate )
{
    m_scriptComponent.Prepare( m_rObjectData.GetGroup(), funcName, paramVec);
    
    // Allow the script to execute and return it's context to the queue
    // for the scripts that don't animate
    if( forceUpdate )
        m_scriptComponent.Update();
}


/************************************************************************
*    desc:  Init the physics                                                           
************************************************************************/
void CSprite2D::InitPhysics()
{
    m_physicsComponent.Init(*this);
}


/************************************************************************
*    desc:  React to what the player is doing
************************************************************************/
void CSprite2D::HandleEvent( const SDL_Event & rEvent )
{
    if( m_upAI )
        m_upAI->HandleEvent( rEvent );

}   // HandleEvent


/************************************************************************
*    desc:  Update the sprite                                                           
************************************************************************/
void CSprite2D::Update()
{
    m_scriptComponent.Update();
    
    if( m_upAI )
        m_upAI->Update();

}   // Update


/************************************************************************
*    desc:  Update the physics                                                           
************************************************************************/
void CSprite2D::PhysicsUpdate()
{
    m_physicsComponent.Update( this );

}   // PhysicsUpdate


/************************************************************************
*    desc:  do the render                                                            
************************************************************************/
void CSprite2D::Render( const CMatrix & matrix )
{
    if( IsVisible() )
        m_visualComponent.Render( m_matrix, matrix );

}   // Render


/************************************************************************
*    desc:  Get the visual component                                                            
************************************************************************/
CVisualComponent2D & CSprite2D::GetVisualComponent()
{
    return m_visualComponent;

}   // GetVisualComponent


/************************************************************************
*    desc:  Get the physics component                                                            
************************************************************************/
CPhysicsComponent2D & CSprite2D::GetPhysicsComponent()
{
    return m_physicsComponent;

}   // GetPhysicsComponent


/************************************************************************
*    desc:  Get the scripting component                                                            
************************************************************************/
CScriptComponent & CSprite2D::GetScriptComponent()
{
    return m_scriptComponent;

}   // GetScriptingComponent


/************************************************************************
*    desc:  Get the object data                                                            
************************************************************************/
const CObjectData2D & CSprite2D::GetObjectData() const
{
    return m_rObjectData;

}   // GetObjectData


/************************************************************************
*    desc:  Get the unique id number
************************************************************************/
int CSprite2D::GetId() const
{
    return m_id;

}   // GetId


/************************************************************************
*    desc:  Set/Get the AI pointer. This class owns the pointer
************************************************************************/
void CSprite2D::SetAI( iAIBase2D * pAIBase )
{
    m_upAI.reset( pAIBase );

    // Handle any initialization in a separate function
    m_upAI->Init();

}   // SetAI


/************************************************************************
*    desc:  Set the color
************************************************************************/
void CSprite2D::SetColor( const CColor & color )
{
    m_visualComponent.SetColor( color );

}   // SetColor

void CSprite2D::SetRGBA( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_visualComponent.SetRGBA( r, g, b, a );

}   // SetRGBA


/************************************************************************
*    desc:  Set the default color
************************************************************************/
void CSprite2D::SetDefaultColor()
{
    m_visualComponent.SetColor( m_rObjectData.GetVisualData().GetColor() );

}   // SetColor


/************************************************************************
*    desc:  Get the color
************************************************************************/
const CColor & CSprite2D::GetColor() const
{
    return m_visualComponent.GetColor();

}   // GetColor


/************************************************************************
*    desc:  Get the default color
************************************************************************/
const CColor & CSprite2D::GetDefaultColor() const
{
    return m_rObjectData.GetVisualData().GetColor();

}   // GetDefaultColor


/************************************************************************
*    desc:  Set the Alpha
************************************************************************/
void CSprite2D::SetAlpha( float alpha )
{
    if( alpha > 1.5 )
        alpha *= defs_RGB_TO_DEC;

    m_visualComponent.SetAlpha( alpha );

}   // SetAlpha


/************************************************************************
*    desc:  Get the Alpha
************************************************************************/
float CSprite2D::GetAlpha() const
{
    return m_visualComponent.GetAlpha();

}   // GetAlpha


/************************************************************************
*    desc:  Get the default alpha
************************************************************************/
float CSprite2D::GetDefaultAlpha() const
{
    return m_rObjectData.GetVisualData().GetColor().GetA();

}   // GetDefaultAlpha


/************************************************************************
*    desc:  Set the default alpha
************************************************************************/
void CSprite2D::SetDefaultAlpha()
{
    m_visualComponent.SetAlpha( m_rObjectData.GetVisualData().GetColor().GetA() );

}   // SetDefaultAlpha


/************************************************************************
*    desc:  Get the frame count
************************************************************************/
uint CSprite2D::GetFrameCount() const 
{
    return m_rObjectData.GetVisualData().GetFrameCount();

}   // GetFrameCount


/************************************************************************
*    desc:  Get the current frame
************************************************************************/
uint CSprite2D::GetCurrentFrame() const 
{
    return m_visualComponent.GetCurrentFrame();

}   // GetFrameCount


/************************************************************************
*    desc:  Set the texture ID from index
************************************************************************/
void CSprite2D::SetFrame( uint index )
{
    if( m_visualComponent.GetCurrentFrame() != index )
    {
        m_visualComponent.SetFrame( index );
        
        if( m_rObjectData.GetVisualData().GetGenerationType() == NDefs::EGT_SPRITE_SHEET )
            SetCropOffset( m_rObjectData.GetVisualData().GetSpriteSheet().GetGlyph(index).GetCropOffset() );
    }

}   // SetFrame


/************************************************************************
*    desc:  Create the font string
************************************************************************/
void CSprite2D::CreateFontString( const std::string & fontString )
{
    m_visualComponent.CreateFontString( fontString );

}   // CreateFontString


/************************************************************************
*    desc:  Get the font size
************************************************************************/
const CSize<float> & CSprite2D::GetFontSize() const
{
    return m_visualComponent.GetFontSize();

}   // CreateFontString


/************************************************************************
*    desc:  Register the class with AngelScript
************************************************************************/
void CSprite2D::Register( asIScriptEngine * pEngine )
{
    using namespace NScriptGlobals;
    
    // Register CScriptComponent2d reference and methods
    Throw( pEngine->RegisterObjectType(  "CSprite2d", 0, asOBJ_REF|asOBJ_NOCOUNT) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetColor(const CColor &in)",                    asMETHOD(CSprite2D, SetColor),             asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetRGBA(float r, float g, float b, float a)",   asMETHOD(CSprite2D, SetRGBA),              asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetDefaultColor()",                             asMETHOD(CSprite2D, SetDefaultColor),      asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "const CColor & GetColor()",                          asMETHOD(CSprite2D, GetColor),             asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "const CColor & GetDefaultColor()",                   asMETHOD(CSprite2D, GetDefaultColor),      asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "uint GetFrameCount()",                               asMETHOD(CSprite2D, GetFrameCount),        asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetFrame(uint index)",                          asMETHOD(CSprite2D, SetFrame),             asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "uint GetCurrentFrame()",                             asMETHOD(CSprite2D, GetCurrentFrame),      asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetAlpha(float alpha)",                         asMETHOD(CSprite2D, SetAlpha),             asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "float GetAlpha()",                                   asMETHOD(CSprite2D, GetAlpha),             asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "float GetDefaultAlpha()",                            asMETHOD(CSprite2D, GetDefaultAlpha),      asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetDefaultAlpha()",                             asMETHOD(CSprite2D, SetDefaultAlpha),      asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void CreateFontString(string &in)",                  asMETHOD(CSprite2D, CreateFontString),     asCALL_THISCALL) );

    // Need to use CObject for the below to compile under MSVSC++
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetVisible(bool visible)",                      asMETHOD(CObject,   SetVisible),           asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "bool IsVisible()",                                   asMETHOD(CObject,   IsVisible),            asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "CPoint GetPos()",                                    asMETHOD(CObject,   GetPos_AngelScript),   asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetPos(CPoint &in)",                            asMETHOD(CObject,   SetPos_AngelScript),   asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetRot(CPoint &in, bool convertToRadians = true)", asMETHOD(CObject,   SetRot),            asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void IncRot(CPoint &in, bool convertToRadians = true)", asMETHOD(CObject,   IncRot),            asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetScale(CPoint &in)",                          asMETHOD(CObject,   SetScale),             asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "const CPoint & GetScale()",                          asMETHOD(CObject,   GetScale),             asCALL_THISCALL) );
    
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetPosXYZ(float x, float y, float z)",          asMETHOD(CObject,   SetPosXYZ),            asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void IncPosXYZ(float x, float y, float z)",          asMETHOD(CObject,   IncPosXYZ),            asCALL_THISCALL) );
    
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetRotXYZ(float x, float y, float z, bool convertToRadians = true)", asMETHOD(CObject, SetRotXYZ), asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void IncRotXYZ(float x, float y, float z, bool convertToRadians = true)", asMETHOD(CObject, IncRotXYZ), asCALL_THISCALL) );
    
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetScaleXYZ(float x, float y, float z)",        asMETHOD(CObject,   SetScaleXYZ),          asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CSprite2d", "void IncScaleXYZ(float x, float y, float z)",        asMETHOD(CObject,   IncScaleXYZ),          asCALL_THISCALL) );
    
    Throw( pEngine->RegisterGlobalFunction("void Spawn(string &in, CSprite2d @)", asMETHOD(CScriptManager, PrepareSpawnObj), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
    
}   // Register
