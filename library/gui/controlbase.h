
/************************************************************************
*    FILE NAME:       controlbase.h
*
*    DESCRIPTION:     Control base class
************************************************************************/

#ifndef __ui_control_base_h__
#define __ui_control_base_h__

// Physical component dependency
#include <2d/object2d.h>

// Game lib dependencies
#include <gui/uicontroldefs.h>
#include <common/dynamicoffset.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward declaration(s)
struct XMLNode;

class CControlBase : public CObject2D, boost::noncopyable
{
public:

    // Constructor
    CControlBase( const std::string & group );

    // Destructor
    virtual ~CControlBase();
    
    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node );
    
    // Get the object group name
    const std::string & GetGroup() const;
    
    // Get/Set the control name
    const std::string & GetName() const;
    
    // Get the control type
    NUIControl::EControlType GetType() const;
    
    // Load the control specific info from XML node
    virtual void LoadControlFromNode( const XMLNode & node ) = 0;
    
    // Set the dynamic position
    void SetDynamicPos();
    
    // Get the faction name
    const std::string & GetFaction() const;
    
private:
    
    // Load the dynamic offset data from node
    void LoadDynamicOffsetFromNode( const XMLNode & node );
    
    // Load a sprite from an XML node
    virtual void LoadSpriteFromNode( const XMLNode & node, size_t & fontSpriteCount ) = 0;
    
protected:
        
    // Object data group name
    std::string m_group;
    
    // Unique string id
    std::string m_name;
    
    // The type of control
    NUIControl::EControlType m_type;
    
    // Control string list
    std::vector<std::string> m_stringVec;
    
    // A name that is applied to similar controls.
    // Provides a way to check for many controls without having to use unique names
    std::string m_faction;
    
    // Dynamic offset
    CDynamicOffset m_dynamicOffset;

};

#endif  // __ui_control_base_h__


