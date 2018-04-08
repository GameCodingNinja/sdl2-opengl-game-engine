
/************************************************************************
*    FILE NAME:       reelstripview.cpp
*
*    DESCRIPTION:     Reel strip view class
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <slot/reelstripview.h>

// Game lib dependencies
#include <slot/slotstripmodel.h>
#include <slot/symbol2d.h>
#include <slot/mathsymbol.h>
#include <slot/stripstop.h>
#include <slot/symbolsetview.h>
#include <objectdata/objectdatamanager.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/matrix.h>
#include <common/sound.h>
#include <managers/soundmanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
*           The visual count is the same as the math's evaluation count.
*           These values can be found in the math file.
************************************************************************/
CReelStripView::CReelStripView( const CSlotStripModel & rSlotStripModel, CSymbolSetView & rSymbolSetView, const int reelId ) :
    m_rSlotStripModel(rSlotStripModel),
    m_rSymbolSetView(rSymbolSetView),
    m_reelId(reelId),
    m_visibleSymbCount(rSlotStripModel.GetEvalIndexVec().size()),
    m_bufferSymbols(0),
    m_spinDir(NSlotDefs::ESD_DOWN),
    m_spin(false),
    m_spinDistance(0.0),
    m_velocity(0.0),
    m_acceleration(0.0),
    m_spinDirVector(0),
    m_spinSymbDist(0),
    m_spinStop(0),
    m_symbAlign(false),
    m_symbAlignCounter(0),
    m_spinState(NSlotDefs::ESS_STOPPED),
    m_disableSpinTimer(false),
    m_pSpinStopSnd(nullptr),
    m_allowStopSounds(true)
{
    // Set the value returned by Expired when the timer is disabled
    m_spinTimer.SetDisableValue( true );
    
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CReelStripView::~CReelStripView()
{
}   // destructor


/************************************************************************
*    desc:  Create the reel strip from data node
************************************************************************/
void CReelStripView::Create( const XMLNode & node, const std::string & group )
{
    // Get the number of buffer symbols
    m_bufferSymbols = std::atoi(node.getAttribute( "bufferSymbols" ));
    
    // Get the size of the symbol
    CSize<float> symbolSize;
    symbolSize.w = std::atoi(node.getAttribute( "symbolWidth" ));
    symbolSize.h = std::atoi(node.getAttribute( "symbolHeight" ));
    
    // Get the spin direction and set the spin direction vector
    m_spinDir = NSlotDefs::ESpinDirection(std::atoi(node.getAttribute( "spinDirection" )));
    m_spinDirVector = -1;
    m_spinSymbDist = symbolSize.h;
    
    if( (m_spinDir == NSlotDefs::ESD_UP) || (m_spinDir == NSlotDefs::ESD_RIGHT)  )
        m_spinDirVector = 1;
    
    if( m_spinDir >= NSlotDefs::ESD_LEFT )
        m_spinSymbDist = symbolSize.w;
    
    // Load the transform data from node
    LoadTransFromNode( node );
    
    // Get the stencil mask object name
    const XMLNode stencilMaskNode = node.getChildNode( "stencilMask" );
    if( !stencilMaskNode.isEmpty() )
    {
        const std::string objectName = stencilMaskNode.getAttribute( "objectName" );

        // Allocate the stencil
        m_upStencilMaskSprite.reset( new CSprite2D( CObjectDataMgr::Instance().GetData2D( group, objectName ) ) );
        
        // Load the transform data
        m_upStencilMaskSprite->LoadTransFromNode( stencilMaskNode );
    }
    
    // Get the sprite list if any
    const XMLNode spriteLstNode = node.getChildNode( "spriteList" );
    if( !spriteLstNode.isEmpty() )
    {
        for( int i = 0; i < spriteLstNode.nChildNode(); ++i )
        {
            const XMLNode spriteNode = spriteLstNode.getChildNode( "sprite", i );
            
            // Get the type of object
            const std::string objectName = spriteNode.getAttribute( "objectName" );
            
            m_spriteDeq.emplace_back( CObjectDataMgr::Instance().GetData2D( group, objectName ) );
            
            // Load the transform data
            m_spriteDeq.back().LoadTransFromNode( spriteNode );
        }
    }
    
    // Get the sprite list if any
    const XMLNode stopSoundNode = node.getChildNode( "stopSound" );
    if( !stopSoundNode.isEmpty() )
    {
        const std::string group = stopSoundNode.getAttribute( "group" );
        const std::string soundId = stopSoundNode.getAttribute( "soundId" );
        
        m_pSpinStopSnd = &CSoundMgr::Instance().GetSound( group, soundId );
    }
    
    // Init the reel strip with symbols
    InitReelStrip();
        
}   // LoadFromNode


/************************************************************************
*    desc:  Init the reel strip with symbols
************************************************************************/
void CReelStripView::InitReelStrip()
{
    const int totalSymbols = m_visibleSymbCount + (m_bufferSymbols * 2);
    const int offset = (((totalSymbols - 1) * m_spinSymbDist) / 2);
    
    // Check that all the symbols on the reel strips have a visual symbol partner
    auto & rMathStripVec = m_rSlotStripModel.GetStripVec();
    for( auto iter : rMathStripVec )
        m_rSymbolSetView.GetSymbol( iter.GetMathSymbol().GetID() );
    
    for( int i = 0; i < totalSymbols; ++i )
    {
        // Push it into the deque and set it's position
        // Get the current stop minus the buffer symbols because we render from top to bottom
        m_symbolDeq.push_back( GetSymbol( m_rSlotStripModel.GetLastStop() - m_bufferSymbols + i ) );
        
        // Are we spinning up/down or left/right?
        if( m_spinDir <= NSlotDefs::ESD_DOWN )
            m_symPosDeq.emplace_back( 0, -(i * m_spinSymbDist) + offset );
        else
            m_symPosDeq.emplace_back( -offset + (i * m_spinSymbDist), 0 );
    }
    
}   // InitReelStrip


/************************************************************************
*    desc:  Get the symbol from the reel strip offset
************************************************************************/
CSymbol2d * CReelStripView::GetSymbol( int stop )
{
    // Get the math symbol
    auto & rMathSymb = m_rSlotStripModel.GetSymbol( stop );
    
    return &m_rSymbolSetView.GetSymbol( rMathSymb.GetID() );
    
}   // GetSymbol


/************************************************************************
*    desc:  Get the cycle result symbol for this spot on the reel strip
*           NOTE: Index assumes visible symbol that can animate
*                 Will need to be freed by the recipient of pointer.
************************************************************************/
CSymbol2d * CReelStripView::GetCycleResultSymbol( int index )
{
    // Get the current symbol ID
    const auto & symbId = m_symbolDeq.at(m_bufferSymbols + index)->GetId();
    
    // allocate a new symbol for the cycle results
    // NOTE: Recipient of this pointer is responsible for deleting it
    auto * pSymbol = new CSymbol2d( m_rSymbolSetView.GetSpriteData( symbId ), symbId );
    
    // Replace the current symbol pointer with the allocated one
    m_symbolDeq.at(m_bufferSymbols + index) = pSymbol;

    return pSymbol;
    
}   // GetCycleResultSymbol


/************************************************************************
*    desc:  This replaces the temporary cycle symbols with the ones used for spinning
************************************************************************/
void CReelStripView::ClearCycleResultSymbs()
{
    for( int i = 0; i < m_visibleSymbCount; ++i )
    {
        const auto & symbId = m_symbolDeq.at(m_bufferSymbols + i)->GetId();
        
        m_symbolDeq.at(i + m_bufferSymbols) = &m_rSymbolSetView.GetSymbol( symbId );
    }
    
}   // ClearCycleResultSymbs


/************************************************************************
*    desc:  Set the spin profile
************************************************************************/
void CReelStripView::SetSpinProfile( const CSpinProfile & spinProfile )
{
    m_spinProfile = spinProfile;
    
}   // SetSpinProfile


/************************************************************************
*    desc:  Update the reel strip
************************************************************************/
void CReelStripView::Update()
{
    if( m_spin )
    {
        switch( m_spinState )
        {
            // Do the spin init and let it fall through to start the spin
            case NSlotDefs::ESS_STOPPED:
            {
                m_spinStop = m_rSlotStripModel.GetLastStop();
                
                // Set the spin stop to the current stop with offset adjustments
                if( (m_spinDir == NSlotDefs::ESD_DOWN) || (m_spinDir == NSlotDefs::ESD_RIGHT) )
                    m_spinStop -= m_bufferSymbols;
                else
                    m_spinStop += m_visibleSymbCount + m_bufferSymbols;
                
                m_velocity = 0.0;
                m_acceleration = m_spinProfile.GetAccelation();
                m_spinTimer.Set( m_spinProfile.GetStartDelay() );
                m_spinState = NSlotDefs::ESS_SPIN_STARTING;
            }
            
            case NSlotDefs::ESS_SPIN_STARTING:
            {
                if( m_spinTimer.Expired() )
                {
                    // Increment the velocity and accelation
                    const float elapsedTime = CHighResTimer::Instance().GetElapsedTime();
                    m_velocity += m_acceleration * elapsedTime;
                    m_acceleration += m_spinProfile.GetImpulse() * elapsedTime;
                    
                    // Cap the velocity at the max speed
                    if( m_velocity >= m_spinProfile.GetMaxVelocity() )
                    {
                        m_velocity = m_spinProfile.GetMaxVelocity();
                        m_spinTimer.Set( m_spinProfile.GetMaxVelocityTime() );
                        m_spinState = NSlotDefs::ESS_SPINNING;
                    }
                    
                    IncSpin( m_velocity );
                }
                
                break;
            }
            
            case NSlotDefs::ESS_SPINNING:
            {
                IncSpin( m_velocity );
                
                // Disable the timer if fast stop is needed
                m_spinTimer.Disable( m_disableSpinTimer );
                
                // Wait for the spin to expire
                if( m_spinTimer.Expired() && m_symbAlign )
                {
                    // Set the new stop to start splicing at the end of the rendered strip
                    m_spinStop = m_rSlotStripModel.GetStop();
                    
                    // Set the spin stop to the current stop with offset adjustments
                    if( (m_spinDir == NSlotDefs::ESD_DOWN) || (m_spinDir == NSlotDefs::ESD_RIGHT) )
                        m_spinStop += m_visibleSymbCount + m_bufferSymbols;
                    else
                        m_spinStop -= m_bufferSymbols + 1;

                    m_symbAlignCounter = 0;
                    
                    m_spinState = NSlotDefs::ESS_PREPARE_TO_STOP;
                }
                
                break;
            }
            
            case NSlotDefs::ESS_PREPARE_TO_STOP:
            {
                IncSpin( m_velocity );
                
                if( m_symbAlign )
                {
                    // Wait for all but the last symbol to get into place
                    if( ++m_symbAlignCounter >= (m_visibleSymbCount + (m_bufferSymbols * 2) - 1) )
                    {
                        // Set a negative accelation slow down the spin to slightly pass that last symbol and cause a bounce up into position
                        m_acceleration = -(m_spinProfile.GetMaxVelocity() / (m_spinSymbDist + m_spinProfile.GetBounceCorrection()));
                        
                        // Set the spin timer as an eror catch for the next state
                        m_spinTimer.Set( m_spinProfile.GetTimeOutDelay() );
                        
                        m_spinState = NSlotDefs::ESS_SPIN_STOPPING;
                    }
                }

                break;
            }
            
            case NSlotDefs::ESS_SPIN_STOPPING:
            {  
                const float elapsedTime = CHighResTimer::Instance().GetElapsedTime();
                m_velocity += m_acceleration * elapsedTime;
                
                // Add in the drag but keep it from turning the value positive
                float drag = m_spinProfile.GetBounceDrag() * elapsedTime;
                if( (m_acceleration + drag) < -0.0 )
                    m_acceleration += drag;

                // Once spin distance goes negative, we are coming back from the bounce.
                // Spin has completed. This will also catch errors
                // The spin timer here is also used as safty valve in the event of an error
                // to force the reel to stop
                if( (m_spinDistance < 0.0) || m_spinTimer.Expired() )
                {
                    // Stop the spin
                    m_velocity = 0.0;
                    m_spin = false;
                    m_disableSpinTimer = false;
    
                    // Hard set the final position of the symbols
                    FinalizeSymbPos();
                    
                    m_spinState = NSlotDefs::ESS_STOPPED;
                    
                    // Send the signal of the spin state
                    m_spinStateSignal(m_reelId, NSlotDefs::ESS_STOPPED);
                    
                    if( m_allowStopSounds && (m_pSpinStopSnd != nullptr) )
                    {
                        const int channel = CSoundMgr::Instance().GetNextChannel();
                        m_pSpinStopSnd->Play( channel );
                    }
                    
                    break;
                }

                IncSpin( m_velocity );
                
                break;
            }
        }
    }
    
}   // Update


/************************************************************************
*    desc:  Inc the reel spin
************************************************************************/
void CReelStripView::IncSpin( float velocity )
{
    CPoint<float> symbOffset;
    
    // Get the distance traveled
    float dist = velocity * CHighResTimer::Instance().GetElapsedTime();
    
    // Are we spinning up/down or left/right?
    if( m_spinDir <= NSlotDefs::ESD_DOWN )
        symbOffset.y = dist;
    else
        symbOffset.x = dist;
    
    // Increment the position of all the symbols
    for( auto & iter : m_symPosDeq )
        iter += symbOffset * m_spinDirVector;

    // Inc for distance checking
    m_spinDistance += dist;
    
    // Clear the alignment flag
    m_symbAlign = false;

    if( m_spinDistance >= m_spinSymbDist )
    {
        // Get the new symbol and set it's position
        if( m_spinDir <= NSlotDefs::ESD_DOWN )
        {
            m_spinStop += m_spinDirVector;
            symbOffset.y = m_spinSymbDist;
        }
        else
        {
            m_spinStop -= m_spinDirVector;
            symbOffset.x = m_spinSymbDist;
        }
        
        auto pSymbol = GetSymbol( m_spinStop );

        // Pop the old symbol and push the new one on based on the way it's spinning
        // Invert the spin direction vector to set the offset of the new symbol
        if( (m_spinDir == NSlotDefs::ESD_DOWN) || (m_spinDir == NSlotDefs::ESD_RIGHT) )
        {
            m_symbolDeq.pop_back();
            m_symbolDeq.push_front( pSymbol );
            
            m_symPosDeq.push_front( m_symPosDeq.front() + (symbOffset * -m_spinDirVector) );
            m_symPosDeq.pop_back();
        }
        else
        {
            m_symbolDeq.pop_front();
            m_symbolDeq.push_back( pSymbol );
            
            m_symPosDeq.push_back( m_symPosDeq.back() + (symbOffset * -m_spinDirVector) );
            m_symPosDeq.pop_front();
        }

        // Reset the spin distance with the remainder
        m_spinDistance -= m_spinSymbDist;
        
        // Set the flag that indicates the reel is close to alignment
        // and a symbol was pushed on and popped off.
        m_symbAlign = true;
    }
        
}   // IncSpin


/************************************************************************
*    desc:  Hard set the final position of the symbols
************************************************************************/
void CReelStripView::FinalizeSymbPos()
{
    const int offset = (((m_symPosDeq.size() - 1) * m_spinSymbDist) / 2);
    
    // Reset the position and the symbols of the strip
    for( size_t i = 0; i < m_symPosDeq.size(); ++i )
    {
        m_symbolDeq.at(i) = GetSymbol( m_rSlotStripModel.GetStop() - m_bufferSymbols + (int)i );
        
        // Are we spinning up/down or left/right?
        if( m_spinDir <= NSlotDefs::ESD_DOWN )
            m_symPosDeq.at(i) = CPoint<float>( 0, -((int)i * m_spinSymbDist) + offset );
        else
            m_symPosDeq.at(i) = CPoint<float>( -offset + ((int)i * m_spinSymbDist), 0 );
    }
    
}   // FinalizeSymbPos


/************************************************************************
*    desc:  Transform the reel strip
************************************************************************/
void CReelStripView::Transform( const CMatrix & matrix, bool tranformWorldPos )
{
    CObject2D::Transform( matrix, tranformWorldPos );
    
    // Transform the mask
    m_upStencilMaskSprite->Transform( GetMatrix(), WasWorldPosTranformed() );
    
    for( auto & iter : m_spriteDeq )
        iter.Transform( GetMatrix(), WasWorldPosTranformed() );
    
}   // Transform


/************************************************************************
*    desc:  Do the render
************************************************************************/
void CReelStripView::Render( const CMatrix & matrix )
{
    for( auto & iter : m_spriteDeq )
        iter.Render( matrix );

    // Disable rendering to the color buffer
    glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
    glDepthMask( GL_FALSE );

    // Start using the stencil
    glEnable( GL_STENCIL_TEST );

    glStencilFunc( GL_ALWAYS, 0x1, 0x1 );
    glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );


    m_upStencilMaskSprite->Render( matrix );


    // Re-enable color
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

    // Where a 1 was not rendered
    glStencilFunc( GL_EQUAL, 0x1, 0x1 );

    // Keep the pixel
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

    // Disable any writing to the stencil buffer
    glDepthMask(GL_TRUE);


    for( size_t i = 0; i < m_symbolDeq.size(); ++i )
    {
        auto symbol = m_symbolDeq.at(i);

        if( !symbol->IsDeferredRender() )
        {
            symbol->SetPos( m_symPosDeq.at(i) );
            symbol->Transform( GetMatrix(), WasWorldPosTranformed() );
            symbol->Render( matrix );
        }
    }


    // Finished using stencil
    glDisable( GL_STENCIL_TEST );

}   // Render


/************************************************************************
*    desc:  do the render
************************************************************************/
void CReelStripView::DeferredRender( const CMatrix & matrix )
{
    if( m_spinState == NSlotDefs::ESS_STOPPED )
    {
        for( size_t i = 0; i < m_symbolDeq.size(); ++i )
        {
            auto symbol = m_symbolDeq.at(i);
            
            if( symbol->IsDeferredRender() )
            {
                symbol->SetPos( m_symPosDeq.at(i) );
                symbol->Transform( GetMatrix(), WasWorldPosTranformed() );
                symbol->Render( matrix );
            }
        }
    }

}   // DeferredRender


/************************************************************************
*    desc:  Start the spin
************************************************************************/
void CReelStripView::StartSpin()
{
    m_spin = true;
    
}   // StartSpin


/************************************************************************
*    desc:  Stop the spin
************************************************************************/
void CReelStripView::StopSpin()
{
    if( m_spin && (m_spinState < NSlotDefs::ESS_SPIN_STOPPING) )
        m_disableSpinTimer = true;
        
}   // StopSpin


/************************************************************************
*    desc:  Get the spin state
************************************************************************/
NSlotDefs::ESpinState CReelStripView::GetSpinState() const
{
    return m_spinState;
    
}   // GetSpinState


/************************************************************************
*    desc:  Connect to the spin state signal
************************************************************************/
void CReelStripView::Connect_SpinState( const SpinStateSignal::slot_type & slot )
{
    m_spinStateSignal.connect(slot);

}   // Connect_SpinState


/************************************************************************
*    desc:  Get the number of visible symbols on this reel
************************************************************************/
int CReelStripView::GetVisibleSymbolCount() const
{
    return m_visibleSymbCount;
    
}   // GetVisibleSymbolCount


/************************************************************************
*    desc:  Do we allow the stop sounds?
************************************************************************/
void CReelStripView::AllowStopSounds( bool allow )
{
    m_allowStopSounds = allow;
}
