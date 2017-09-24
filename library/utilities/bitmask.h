
/************************************************************************
*    FILE NAME:       bitmask.h
*
*    DESCRIPTION:     Class to handle a bit mask
************************************************************************/

#ifndef __bit_mask_h__
#define __bit_mask_h__

// Game lib dependencies
#include <common/defs.h>

template <typename type>
class CBitmask
{
public:

    // Constructor
    CBitmask() : bitmask(0)
    {}

    // Copy Constructors
    CBitmask( const type args ) : bitmask(args)
    {}
    
    template <typename U>
    CBitmask( const CBitmask<U> bitmask ) : bitmask(bitmask.bitmask)
    {}

    
    /************************************************************************
    *    desc:  Add the parameters to the bitmask
    ************************************************************************/
    void Add( const type args )
    {
        bitmask |= args;

    }   // Set


    /************************************************************************
    *    desc:  Remove the parameters from the bitmask
    ************************************************************************/
    void Remove( const type args )
    {
        bitmask &= args ^ -1;

    }   // Remove


    /************************************************************************
    *    desc:  Remove all parameters except for the ones passed in
    ************************************************************************/
    void RemoveAllExcept( const type args )
    {
        bitmask &= args;

    }   // RemoveAllExcept


    /************************************************************************
    *    desc:  Set the bitmask to zero
    ************************************************************************/
    void Clear()
    {
        bitmask = 0;

    }   // Clear
    
    
    /************************************************************************
    *    desc:  Check if all of the parameters are set
    ************************************************************************/
    bool IsEmpty() const
    {
        return (bitmask == 0);

    }   // AreAllSet


    /************************************************************************
    *    desc:  Check if one of the parameters is set
    ************************************************************************/
    bool IsSet( const type args ) const
    {
        return (bitmask & args) != 0;

    }   // IsSet


    /************************************************************************
    *    desc:  Check if all of the parameters are set
    ************************************************************************/
    bool AreAllSet( const type args ) const
    {
        return (bitmask & args) == args;

    }   // AreAllSet


    /************************************************************************
    *    desc:  Get a copy of the bitmask including the parameters
    ************************************************************************/
    type GetIncluding( const type args ) const
    {
        return bitmask | args;

    }   // GetIncluding


    /************************************************************************
    *    desc:  Get a copy of the bitmask excluding the parameters
    ************************************************************************/
    type GetExcluding( const type args ) const
    {
        return bitmask & (args ^ -1);

    }   // GetExcluding


    /************************************************************************
    *    desc:  Overloaded assignment operator
    ************************************************************************/
    void operator = ( const type value )
    {
        bitmask = value;

    }   // operator =

private:

    // bit mask
    type bitmask;

};

#endif  // __bit_mask_h__
