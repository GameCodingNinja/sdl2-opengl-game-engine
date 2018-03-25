/************************************************************************
*    FILE NAME:       mesh3d.h
*
*    DESCRIPTION:     3D mesh class
************************************************************************/  

#ifndef __mesh_3d_h__
#define __mesh_3d_h__

// Game lib dependencies
#include <common/mesh.h>

// Standard lib dependencies
#include <vector>

class CMesh3D
{
public:

    bool meshEmpty() const
    { return m_meshVec.empty(); }
    
    bool textEmpty() const
    { return m_meshVec.back().m_textureVec.empty(); }
    
    size_t size() const
    { return m_meshVec.size(); }
    
    CMesh & back()
    { return m_meshVec.back(); }
    
    const CMesh & back() const
    { return m_meshVec.back(); }

    void reserve( size_t value )
    { m_meshVec.reserve(value); }
    
    void emplace_back()
    { m_meshVec.emplace_back(); }
    
    void emplace_back( const CMesh & mesh )
    { m_meshVec.emplace_back( mesh ); }
    
    std::vector<CMesh> & getVec()
    { return m_meshVec; }
    
    const std::vector<CMesh> & getVec() const
    { return m_meshVec; }

private:
    
    // Loaded mesh data
    std::vector<CMesh> m_meshVec;

};

#endif  // __mesh_3d_h__
