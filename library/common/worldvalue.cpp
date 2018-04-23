
/************************************************************************
*    FILE NAME:       worldvalue.cpp
*
*    DESCRIPTION:     World value class
************************************************************************/

// Physical component dependency
#include <common/worldvalue.h>

// Game lib dependencies
#include <utilities/settings.h>

// Standard lib dependencies
#include <math.h>
#include <complex>

int CWorldValue::SECTOR_SIZE;
int CWorldValue::HALF_SECTOR_SIZE;

/************************************************************************
*    desc:  Constructor
************************************************************************/
CWorldValue::CWorldValue() :
    f(0),
    i(0)
{
}

CWorldValue::CWorldValue( const CWorldValue & value )
{
    i = value.i;
    f = value.f;
    conformValue();
}

CWorldValue::CWorldValue( int _i )
    : i(0)
{
    f = static_cast<float>(_i);
    conformValue();
}

CWorldValue::CWorldValue( float _f )
    : i(0)
{
    f = _f;
    conformValue();
}

CWorldValue::CWorldValue( int _i, float _f )
{
    i = _i;
    f = _f;
    conformValue();
}

CWorldValue::CWorldValue( float _f, int _i )
{
    i = _i;
    f = _f;
    conformValue();
}


/************************************************************************
*    desc:  Conform the value to the sector size
************************************************************************/
void CWorldValue::conformValue()
{
    if( SECTOR_SIZE > 0 && std::abs(f) > HALF_SECTOR_SIZE )
    {
        // Adjusted absolute value
        int iAdjust = 0;

        if( f > 0 )
            iAdjust = int( ( f / float(SECTOR_SIZE) ) + 0.5f );
        else
            iAdjust = int( ( f / float(SECTOR_SIZE) ) - 0.5f );

        f -= iAdjust * SECTOR_SIZE;
        i += iAdjust;
    }
}


/************************************************************************
*    desc:  Return a copy of the value conformed to the sector size
************************************************************************/
CWorldValue CWorldValue::getConformedValue() const
{
    return CWorldValue(i,f);
}


/************************************************************************
*    desc:  Convert the world value in single float form
************************************************************************/
void CWorldValue::toFloat()
{
    if( i != 0 )
    {
        f += SECTOR_SIZE * i;
        i = 0;
    }
}


/************************************************************************
*    desc:  Return a copy of the world value in single float form
*
*	 ret:	float - equal float value
************************************************************************/
float CWorldValue::getFloat() const
{
    if( i == 0 )
        return f;

    return f + (SECTOR_SIZE * i);
}


/***********************************************************************************
*	COMPARISON OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Equality operator
************************************************************************/
bool CWorldValue::operator == ( const CWorldValue & value ) const
{
    if( i == value.i && f == value.f )
        return true;

    return false;
}

bool CWorldValue::operator == ( const float value ) const
{
    if( getFloat() == value )
        return true;

    return false;
}


/************************************************************************
*    desc:  Inequality operator
************************************************************************/
bool CWorldValue::operator != ( const CWorldValue & value ) const
{
    if( i != value.i || f != value.f )
        return true;

    return false;
}

bool CWorldValue::operator != ( const float value ) const
{
    if( getFloat() != value )
        return true;

    return false;
}


/************************************************************************
*    desc:  Less than operator
************************************************************************/
bool CWorldValue::operator < ( const CWorldValue & value ) const
{
    if( i < value.i )
        return true;

    if( i == value.i && f < value.f )
        return true;

    return false;
}

bool CWorldValue::operator < ( const float value ) const
{
    if( getFloat() < value )
        return true;

    return false;
}


/************************************************************************
*    desc:  Greater than operator
************************************************************************/
bool CWorldValue::operator > ( const CWorldValue & value ) const
{
    if( i > value.i )
        return true;

    if( i == value.i && f > value.f )
        return true;

    return false;
}

bool CWorldValue::operator > ( const float value ) const
{
    if( getFloat() > value )
        return true;

    return false;
}


/************************************************************************
*    desc:  Less than or equal to operator
************************************************************************/
bool CWorldValue::operator <= ( const CWorldValue & value ) const
{
    if( i < value.i )
        return true;

    if( i == value.i && f <= value.f )
        return true;

    return false;
}

bool CWorldValue::operator <= ( const float value ) const
{
    if( getFloat() <= value )
        return true;

    return false;
}


/************************************************************************
*    desc:  Greater than or equal to operator
************************************************************************/
bool CWorldValue::operator >= ( const CWorldValue & value ) const
{
    if( i > value.i )
        return true;

    if( i == value.i && f >= value.f )
        return true;

    return false;
}

bool CWorldValue::operator >= ( const float value ) const
{
    if( getFloat() >= value )
        return true;

    return false;
}


/***********************************************************************************
*	ASSIGNMENT OPERATOR
***********************************************************************************/
/************************************************************************
*    desc:  Assignment operator
************************************************************************/
CWorldValue & CWorldValue::operator = ( const CWorldValue & value )
{
    i = value.i;
    f = value.f;

    return *this;
}


/***********************************************************************************
*	ADDITION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Addition operator
************************************************************************/
CWorldValue CWorldValue::operator + ( const CWorldValue & value ) const
{
    return CWorldValue( i + value.i, f + value.f );
}

CWorldValue CWorldValue::operator + ( const float value ) const
{
    return *this + CWorldValue(value);
}

/************************************************************************
*    desc:  Addition operator
************************************************************************/
void CWorldValue::operator += ( const CWorldValue & value )
{
    *this = *this + value;
}

void CWorldValue::operator += ( const float value )
{
    *this = *this + value;
}


/***********************************************************************************
*	SUBTRACTION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Subtraction operator
************************************************************************/
CWorldValue CWorldValue::operator - ( const CWorldValue & value ) const
{
    return CWorldValue( i - value.i, f - value.f );
}

CWorldValue CWorldValue::operator - ( const float value ) const
{
    return *this - CWorldValue(value);
}

/************************************************************************
*    desc:  Subtraction operator
************************************************************************/
void CWorldValue::operator -= ( const CWorldValue & value )
{
    *this = *this - value;
}

void CWorldValue::operator -= ( const float value )
{
    *this = *this - value;
}


/***********************************************************************************
*	MULTIPLICATION OPERATORS
***********************************************************************************/
/************************************************************************
*    desc:  Multiplication operator
************************************************************************/
CWorldValue CWorldValue::operator * ( const CWorldValue & value ) const
{
    return *this * value.getFloat();
}

CWorldValue CWorldValue::operator * ( const int value ) const
{
    return CWorldValue( i * value, f * value );
}

CWorldValue CWorldValue::operator * ( const float value ) const
{
    CWorldValue tmp;
    float intMultiple;

    intMultiple = float(i) * value;
    tmp.i = int(intMultiple);
    intMultiple -= tmp.i;

    tmp.f = ( f * value ) + ( intMultiple * SECTOR_SIZE );

    tmp.conformValue();

    return tmp;
}

/************************************************************************
*    desc:  Multiplication operator
************************************************************************/
void CWorldValue::operator *= ( const CWorldValue & value )
{
    *this = *this * value;
}

void CWorldValue::operator *= ( const int value )
{
    *this = *this * value;
}

void CWorldValue::operator *= ( const float value )
{
    *this = *this * value;
}


/***********************************************************************************
*	DIVISION OPERATORS
*
*	WARNING: Using any division against a float value will result in an imprecise
*			 answer if the integer value is very high
***********************************************************************************/
/************************************************************************
*    desc:  Division operator
************************************************************************/
CWorldValue CWorldValue::operator / ( const CWorldValue & value ) const
{
    return *this / value.getFloat();
}

CWorldValue CWorldValue::operator / ( const float value ) const
{
    CWorldValue tmp;
    double intMultiple;

    intMultiple = double(i) / (double)value;
    tmp.i = int(intMultiple);
    intMultiple -= tmp.i;

    tmp.f = ( f / value ) + static_cast<float>( intMultiple * SECTOR_SIZE );

    tmp.conformValue();

    return tmp;
}

/************************************************************************
*    desc:  Division operator
************************************************************************/
void CWorldValue::operator /= ( const CWorldValue & value )
{
    *this = *this / value;
}

void CWorldValue::operator /= ( const float value )
{
    *this = *this / value;
}


/***********************************************************************************
*	NEGATIVE OPERATOR
***********************************************************************************/
/************************************************************************
*    desc:  Return a copy of the value with its signs flipped
************************************************************************/
CWorldValue CWorldValue::operator - () const
{
    return CWorldValue(-i,-f);
}


/************************************************************************
*    desc:  Set the sector size
************************************************************************/
void CWorldValue::setSectorSize( int size )
{
    SECTOR_SIZE = size;
    HALF_SECTOR_SIZE = size / 2;
}