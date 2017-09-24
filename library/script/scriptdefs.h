
/************************************************************************
*    FILE NAME:       scriptdefs.h
*
*    DESCRIPTION:     Class template
************************************************************************/

#ifndef __script_defs_h__
#define __script_defs_h__

// Game lib dependencies
#include <common/defs.h>

// Forward declaration(s)
class CVisualComponent2D;

class CScriptParam
{
public:
    
    enum EParamType
    {
        EPT_NULL,
        EPT_BOOL,
        EPT_INT,
        EPT_UINT,
        EPT_FLOAT,
        EPT_REG_OBJ, // Registered Object
    };

    // Constructors
    CScriptParam()             : m_paramVal(0),   m_type(EPT_NULL)    {}
    CScriptParam( bool val )   : m_paramVal(val), m_type(EPT_BOOL)    {}
    CScriptParam( int val )    : m_paramVal(val), m_type(EPT_INT)     {}
    CScriptParam( uint val )   : m_paramVal(val), m_type(EPT_UINT)    {}
    CScriptParam( float val )  : m_paramVal(val), m_type(EPT_FLOAT)   {}
    CScriptParam( void * val ) : m_paramVal(val), m_type(EPT_REG_OBJ) {}

private:
    
    union UParamValue
    {
        bool boolVal;
        int intVal;
        uint uintVal;
        float floatVal;
        void * pRegObjVal;
        
        UParamValue(bool val)   : boolVal(val)    {}
        UParamValue(int val)    : intVal(val)     {}
        UParamValue(uint val)   : uintVal(val)    {}
        UParamValue(float val)  : floatVal(val)   {}
        UParamValue(void * val) : pRegObjVal(val) {}
    };
    
    UParamValue m_paramVal;
    
    EParamType m_type;
    
public:
    
    template<typename T>
    T Get() const;
    
    EParamType GetType() const
    { return m_type; }
};

template<> inline bool   CScriptParam::Get() const { return m_paramVal.boolVal; }
template<> inline int    CScriptParam::Get() const { return m_paramVal.intVal; }
template<> inline uint   CScriptParam::Get() const { return m_paramVal.uintVal; }
template<> inline float  CScriptParam::Get() const { return m_paramVal.floatVal; }
template<> inline void * CScriptParam::Get() const { return m_paramVal.pRegObjVal; }

#endif  // __script_defs_h__


