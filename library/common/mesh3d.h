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
    { return meshVec.empty(); }
    
    bool textEmpty() const
    { return meshVec.back().m_textureVec.empty(); }
    
    size_t size() const
    { return meshVec.size(); }
    
    const CMesh & back() const
    { return meshVec.back(); }

    // Loaded texture data
    std::vector<CMesh> meshVec;

};

#endif  // __mesh_3d_h__
