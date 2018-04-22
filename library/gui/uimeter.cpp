/************************************************************************
*    FILE NAME:       uimeter.cpp
*
*    DESCRIPTION:     Class for user interface meters
************************************************************************/

// Physical component dependency
#include <gui/uimeter.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/exceptionhandling.h>
#include <2d/visualcomponent2d.h>
#include <2d/sprite2d.h>
#include <objectdata/objectdata2d.h>

// Boost lib dependencies
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// Standard lib dependencies
#include <iostream>
#include <chrono>
#include <map>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CUIMeter::CUIMeter( const std::string & group ) :
    CUIControl( group ),
    m_currentValue(0.0),
    m_targetValue(0),
    m_velocity(0.0),
    m_acceleration(0.0),
    m_impulse(0.0),
    m_lastValue(0),
    m_fastBangTime(0.0),
    m_bangUp(false),
    m_pSprite(nullptr),
    m_scaleType(EST_AXIS)
{
    m_type = NUIControl::ECT_METER;

}   // Constructor


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CUIMeter::~CUIMeter()
{
}   // Destructor


/************************************************************************
*    desc:  Load the control info from XML node
*
*    param: node - XML node
************************************************************************/
void CUIMeter::LoadFromNode( const XMLNode & node )
{
    CUIControl::LoadFromNode( node );
    
    // Get the bang range info
    const XMLNode bangRangeNode = node.getChildNode( "bangRange" );
    if( !bangRangeNode.isEmpty() )
    {
        std::map<std::string, EBangType> bangTypeMap
            {{"rampUp", EBT_RAMP_UP}, {"linear", EBT_LINEAR}, {"hybrid", EBT_HYBRID}};
        
        m_bangRangeVec.reserve( bangRangeNode.nChildNode() );
        
        // Get the fast bang time
        m_fastBangTime = std::atoi(bangRangeNode.getAttribute( "fastBangTime" ));
        
        // Set the scale type - How the font is scaled to fit within the meter
        if( std::strcmp( bangRangeNode.getAttribute("scaleType"), "accurate" ) == 0 )
            m_scaleType = EST_ACCURATE;
        
        for( int i = 0; i < bangRangeNode.nChildNode(); ++i )
        {
            const XMLNode rangeNode = bangRangeNode.getChildNode(i);

            m_bangRangeVec.emplace_back(
                std::atoi(rangeNode.getAttribute( "target" )),
                bangTypeMap[ rangeNode.getAttribute( "bangUpType" ) ],
                std::atof(rangeNode.getAttribute( "velocity" )),
                std::atof(rangeNode.getAttribute( "estimatedTime" )),
                std::atof(rangeNode.getAttribute( "slowStartTime" )) );
        }
    }
    
    // Get the max size of the font string to fit within this meter.
    // As the string get's bigger, it will be scaled to fit.
    m_maxFontStrSize = NParseHelper::LoadSize( node );
    
}   // LoadFromNode


/************************************************************************
*    desc:  Load the control specific info from XML node
************************************************************************/
void CUIMeter::LoadControlFromNode( const XMLNode & controlNode )
{
    // Call the parent
    CUIControl::LoadControlFromNode( controlNode );
    
    // Find the sprite that renders the font
    for( auto & iter : m_spriteDeq )
    {
        if( iter.GetVisualComponent().IsFontSprite() )
        {
            m_pSprite = &iter;
            break;
        }
    }
    
    if( m_pSprite == nullptr )
        throw NExcept::CCriticalException("UI Meter Init Error!",
            boost::str( boost::format("UI Meter doesn't have a sprite for rendering a font string (%s).\n\n%s\nLine: %s")
                % m_name % __FUNCTION__ % __LINE__ ));

}   // LoadControlFromNode


/************************************************************************
*    desc:  Set the amount to the meter without banging up
************************************************************************/
void CUIMeter::Set( const double amount )
{
    if( ((int64_t)amount > 0) && ((int64_t)amount != (int64_t)m_currentValue) )
    {
        m_lastValue = m_currentValue;
        m_currentValue = m_targetValue = amount;
        
        // Display the value in the meter
        DisplayValue();
    }
    
}   // Set


/************************************************************************
*    desc:  Start the bang range
************************************************************************/
void CUIMeter::StartBangUp( const double amount )
{
    if( (int64_t)amount != (int64_t)m_currentValue )
    {
        m_targetValue = amount;
        m_bangUp = true;

        SetBangRange();
    }
    
}   // StartBangUp


/************************************************************************
*    desc:  Set the bang range
************************************************************************/
void CUIMeter::SetBangRange()
{
    bool found(false);
    
    for( auto & iter : m_bangRangeVec )
    {
        if( (int64_t)(m_targetValue - m_currentValue) <= iter.m_target )
        {
            found = true;
            InitBangRange( iter );
            break;
        }
    }
    
    if( !found )
        InitBangRange( m_bangRangeVec.back() );
    
}   // SetBangRange


/************************************************************************
*    desc:  Init the bang range
************************************************************************/
void CUIMeter::InitBangRange( const CBangRange & bangRange )
{
    m_bangRange = bangRange;
    m_terminalVelocity = 0.0;
    m_acceleration = 0.0;
    m_impulse = 0.0;
    m_bangScaleAdjustment.set(1,1);
    
    m_pSprite->SetScale( CPoint<float>(1,1,1) );
    
    m_velocity = bangRange.m_velocity / 1000.0;
    
    double range( m_targetValue - m_currentValue );
    
    // Ramp up from start to finish
    if( bangRange.m_bangType == EBT_RAMP_UP )
    {
        m_impulse = range / (bangRange.m_estimatedTime * bangRange.m_estimatedTime * 1000.0);
        m_acceleration = m_impulse;
    }
    // Linear bang up from the start
    else if( bangRange.m_bangType == EBT_LINEAR )
    {
        m_acceleration = range / (bangRange.m_estimatedTime * 1000.0);
    }
    // combination of ramp up and linear
    else if( bangRange.m_bangType == EBT_HYBRID )
    {
        m_terminalVelocity = range / (bangRange.m_estimatedTime * 1000.0);
        m_impulse = range / (bangRange.m_estimatedTime * bangRange.m_estimatedTime * 500.0);
        m_acceleration = m_impulse;
    }
    
    // Set the timer to allow the bang-up to start off slowly
    m_startUpTimer.Set( bangRange.m_slowStartTime );
    
    // Prepare the start script function if one exists
    m_pSprite->PrepareFuncId( "start" );
    
}   // InitBangRange


/************************************************************************
*    desc:  Do a fast bang
************************************************************************/
void CUIMeter::FastBang()
{
    if( m_bangUp )
    {
        double acceleration = (m_targetValue - m_currentValue) / m_fastBangTime;
        
        // use the fast bang acceleration if the current one is less
        if( m_acceleration < acceleration )
            m_acceleration = acceleration;
    }
    
}   // FastBang


/************************************************************************
*    desc:  Update the control
************************************************************************/
void CUIMeter::Update()
{
    CUIControl::Update();
    
    if( m_bangUp )
    {
        const double elapsedTime = CHighResTimer::Instance().GetElapsedTime();
        
        // Ramp up from start to finish
        if( m_bangRange.m_bangType == EBT_RAMP_UP )
        {
            m_currentValue += m_velocity * elapsedTime;
            
            if( m_startUpTimer.Expired() )
            {
                m_velocity += m_acceleration * elapsedTime;
                m_acceleration += m_impulse * elapsedTime;
            }
            else
            {
                m_velocity += m_acceleration * elapsedTime;
            }
        }
        // Linear bang up from the start
        else if( m_bangRange.m_bangType == EBT_LINEAR )
        {
            m_currentValue += m_velocity;
            
            if( m_startUpTimer.Expired() )
                m_velocity += m_acceleration * elapsedTime;
        }
        // combination of ramp up and linear
        else if( m_bangRange.m_bangType == EBT_HYBRID )
        {
            m_currentValue += m_velocity;
            
            if( m_startUpTimer.Expired() )
            {
                if( m_terminalVelocity > m_acceleration )
                {
                    m_velocity += m_acceleration * elapsedTime;
                    m_acceleration += m_impulse * elapsedTime;
                }
                else
                {
                    m_velocity += m_acceleration * elapsedTime;
                }
            }
            else
            {
                m_velocity += m_acceleration * elapsedTime;
            }
        }
        
        // Only update the meter if the value is different
        if( (int64_t)m_lastValue != (int64_t)m_currentValue )
        {
            m_lastValue = m_currentValue;
            
            // check if the bang up has finished
            if( m_currentValue > m_targetValue )
            {
                m_currentValue = m_targetValue;
                m_bangUp = false;
                
                // Prepare the stop script function if one exists
                m_pSprite->PrepareFuncId( "stop" );
            }
        
            // Display the value in the meter
            DisplayValue();
        }
    }

}   // Update


/************************************************************************
*    desc:  Display the value in the meter
************************************************************************/
void CUIMeter::DisplayValue()
{
    // Display the new value
    m_pSprite->CreateFontString( boost::lexical_cast<std::string>((int64_t)m_currentValue ) );

    // Get the font size
    const CSize<float> & size = m_pSprite->GetFontSize();

    // Check if the font string size is greater then what is allowed
    if( size > m_maxFontStrSize )
    {
        const CSize<float> dif = m_maxFontStrSize / size;

        // Is the difference less then the last size change
        if( dif < m_bangScaleAdjustment )
        {
            m_bangScaleAdjustment = dif;

            CPoint<float> scale( m_pSprite->GetScale() );
            if( dif.w < dif.h )
                scale.set(dif.w, (m_scaleType == EST_AXIS ? 1.f : dif.w), 1.f);
            else
                scale.set((m_scaleType == EST_AXIS ? 1.f : dif.h), dif.h, 1.f);

            m_pSprite->SetScale( scale );
        }
    }
            
}   // DisplayValue


/************************************************************************
*    desc:  Is the meter banging
************************************************************************/
bool CUIMeter::IsBanging()
{
    return m_bangUp;
    
}   // IsBanging


/************************************************************************
*    desc:  Clear the meter
************************************************************************/
void CUIMeter::Clear()
{
    m_lastValue = m_currentValue = m_targetValue = 0;
    m_bangUp = false;
    
    if( !m_pSprite->PrepareFuncId( "clear" ) )
    {
        m_pSprite->CreateFontString( boost::lexical_cast<std::string>((int64_t)m_currentValue ) );
    }

}   // Clear
