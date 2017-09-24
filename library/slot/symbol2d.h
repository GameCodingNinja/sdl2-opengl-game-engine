
/************************************************************************
*    FILE NAME:       symbol2d.h
*
*    DESCRIPTION:     Class for rendering a symbol and it's animations
************************************************************************/

#ifndef __symbol_2d_h__
#define __symbol_2d_h__

// Physical component dependency
#include <2d/object2d.h>

// Game lib dependencies
#include <2d/spritechild2d.h>

// Standard lib dependencies
#include <deque>

// Forward declaration(s)
class CSpriteData;

class CSymbol2d : public CObject2D
{
public:

    // Constructor
    CSymbol2d( const std::vector<CSpriteData> & rSpriteDataVec, const std::string & id );

    // Destructor
    virtual ~CSymbol2d();
    
    // Get the sprite
    CSprite2D & GetSprite( int index = 0 );
    
    // Get the sprite
    size_t GetSpriteCount();
    
    // Get the string id
    const std::string & GetId();
    
    // Update the control
    void Update();

    // Transform the control
    void Transform() override;
    void Transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;

    // do the render
    void Render( const CMatrix & matrix );
    
private:
    
    // final matrix
    CMatrix m_finalMatrix;
    
    // This symbol's visual representation
    std::deque<CSpriteChild2D> m_spriteDeq;
    
    // string Id;
    std::string m_id;
};

#endif  // __symbol_2d_h__


