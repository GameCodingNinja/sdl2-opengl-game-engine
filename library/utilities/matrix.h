/************************************************************************
*    FILE NAME:       matrix.h
*
*    DESCRIPTION:     4x4 Matrix math class
************************************************************************/

#ifndef __matrix_h__
#define __matrix_h__

// Game lib dependencies
#include <common/normal.h>
#include <common/quaternion.h>
#include <common/rect.h>
#include <common/size.h>
#include <common/quad.h>
#include <common/point.h>

// Standard lib dependencies
#include <cstdint>

enum
{
    m00, m01, m02, m03,
    m10, m11, m12, m13,
    m20, m21, m22, m23,
    m30, m31, m32, m33,
    mMax
};

// The 3D view matrix class:
class CMatrix
{
public:
    // Constructor
    CMatrix();

    // Copy constructor
    CMatrix( const CMatrix & obj );
    CMatrix( float mat[mMax] );
    
    // Copy from another matrix
    void Copy( const CMatrix & obj );

    // Reset the matrix to the identity matrix
    void InitilizeMatrix();

    // Clear translation data from the matrix
    void ClearTranlate();

    // Generate 3D rotation matrix
    void Rotate( const CPoint<float> & point );

    // Get the Z rotation of the matrix
    float GetZRot( bool inDegrees = true ) const;

    // Create 3D translation matrix
    void Translate( const CPoint<float> & point );
    void Translate( const CSize<int16_t> & size );

    // Function designed to merge scaling matrix with master matrix
    void SetScale( const CPoint<float> & point );
    void SetScale( const CSize<float> & size );
    void SetScale( float scale );
    void Scale( const CPoint<float> & point );
    void Scale( const CSize<float> & size );
    void Scale( float scale );

    // Functions designed to transform using the master matrix
    void Transform( CPoint<float> & dest, const CPoint<float> & source ) const;
    void Transform( CPoint<float> * pDest, const CPoint<float> * pSource ) const;
    void Transform( CNormal<float> & dest, const CNormal<float> & source ) const;
    void Transform( CRect<float> & dest, const CRect<float> & source ) const;
    void Transform( CQuad & dest, const CQuad & source ) const;
    void Transform3x3( CPoint<float> & dest, const CPoint<float> & source ) const;
    
    // Merge matrix into master matrix
    void MergeMatrix( const CMatrix & obj );

    // Get the transpose of a matrix
    CMatrix GetTransposeMatrix() const;

    // Get the matrix point in space
    CPoint<float> GetMatrixPoint();

    // Get matrix rotation
    CPoint<float> GetMatrixRotation();

    // Inverses this matrix.
    void InvertX();
    void InvertY();
    void InvertZ();
    bool Inverse();

    // Set quaternion
    void Set( const CQuaternion & quat );
    
    // Use a point to set a row
    void SetRow( int row, const CPoint<float> & point );
    
    // Use a point to set a column
    void SetColumn( int col, const CPoint<float> & point );

    // Create the matrix based on where the camera is looking
    void LookAt( const CPoint<float> & pos, const CPoint<float> & target, const CPoint<float> & cameraUp );

    // Multiply the matrices only using the rotation/scale portion
    void Multiply3x3( const CMatrix & obj );

    // Calulate an orthographic matrix
    void OrthographicRH( float w, float h, float zn, float zf );
    void OrthographicLH( float w, float h, float zn, float zf );

    // Calulate an perspective matrix
    void PerspectiveFovRH( float fovy, float aspect, float zn, float zf );
    void PerspectiveFovLH( float fovy, float aspect, float zn, float zf );

    // Overloaded operators
    CMatrix operator * ( const CMatrix & obj ) const;
    CMatrix operator *= ( const CMatrix & obj );
    CMatrix operator = ( const float mat[4][4] );
    const float operator [] ( uint index ) const;
    float & operator [] ( uint index );
    const float * operator () () const;

private:

    // Initializes a specific matrix to the identity matrix
    void InitIdentityMatrix( float mat[mMax] );

    // Merge source matrix into destination matrix
    void MergeMatrices( float dest[mMax], const float source[mMax] );

    // Merge matrix into master matrix
    void MergeMatrix( const float newMatrix[mMax] );
    void ReverseMergeMatrix( const float newMatrix[mMax] );
    void MergeMatrix3x3( const float mat[mMax] );

    // Separate axis rotation functions
    void RotateZ( float dest[mMax], float value, int rotFlags );
    //void RotateZDeg( float dest[mMax], float value, int rotFlags );
    void RotateY( float dest[mMax], float value, int rotFlags );
    //void RotateYDeg( float dest[mMax], float value, int rotFlags );
    void RotateX( float dest[mMax], float value, int rotFlags );
    //void RotateXDeg( float dest[mMax], float value, int rotFlags );
    
    // scale and merge
    void MergeScale( float x, float y, float z );

private:

    // Master matrix
    // a single dimension array (array[]) is much faster
    // then a multi-dimension array (array[][])
    float matrix[mMax];

};


#endif  // __matrix_h__