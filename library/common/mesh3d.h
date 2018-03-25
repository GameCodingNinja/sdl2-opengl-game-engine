/************************************************************************
*    FILE NAME:       mesh3d.h
*
*    DESCRIPTION:     3D mesh class
************************************************************************/  

#ifndef __mesh_3d_h__
#define __mesh_3d_h__

// Game lib dependencies
#include <common/mesh.h>
#include <common/point.h>
#include <common/normal.h>
#include <common/uv.h>

// Standard lib dependencies
#include <vector>
#include <memory>

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
    
    CMeshBinaryFileHeader & getFileHeader()
    { return m_fileHeader; }
    
    uint32_t getVerifyValue()
    { return m_fileHeader.file_header; }
    
    uint16_t getVertCount()
    { return m_fileHeader.vert_count; }
    
    uint16_t getUVCount()
    { return m_fileHeader.uv_count; }
    
    uint16_t getNormCount()
    { return m_fileHeader.vert_norm_count; }
    
    uint16_t getFaceGroupCount()
    { return m_fileHeader.face_group_count; }
    
    uint16_t getTextCount()
    { return m_fileHeader.text_count; }
    
    uint16_t getJointCount()
    { return m_fileHeader.joint_count; }
    
    void allocateArrays()
    {
        m_spVert.reset( new CPoint<float>[getVertCount()] );
        m_spVNormal.reset( new CNormal<float>[getNormCount()] );
        
        if( getUVCount() != 0 )
            m_spUV.reset( new CUV[getUVCount()] );
    }
    
    void clearArrays()
    {
        m_spVert.reset();
        m_spVNormal.reset();
        m_spUV.reset();
        
        for( auto & iter : m_meshVec )
            iter.clearArrays();
    }
    
    CPoint<float> * getVertAry()
    { return m_spVert.get(); }
    
    CNormal<float> * getNormalAry()
    { return m_spVNormal.get(); }
    
    CUV * getUVAry()
    { return m_spUV.get(); }

private:
    
    // Mesh file header
    CMeshBinaryFileHeader m_fileHeader;
    
    // Data arrays
    std::shared_ptr<CPoint<float>> m_spVert;
    std::shared_ptr<CNormal<float>> m_spVNormal;
    std::shared_ptr<CUV> m_spUV;
    
    // Loaded mesh data
    std::vector<CMesh> m_meshVec;

};

#endif  // __mesh_3d_h__
