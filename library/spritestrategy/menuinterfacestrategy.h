
/************************************************************************
*    FILE NAME:       menuinterfacestrategy.h
*
*    DESCRIPTION:     Menu interface strategy
************************************************************************/

#ifndef __menu_interface_strategy_h__
#define __menu_interface_strategy_h__

// Physical component dependency
#include <spritestrategy/ispritestrategy.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

class CMenuInterfaceStrategy : public iSpriteStrategy, boost::noncopyable
{
public:

    // Constructor
    CMenuInterfaceStrategy();

    // Destructor
    virtual ~CMenuInterfaceStrategy(){}

    // Update the actors
    virtual void update() override;

    // Transform the actor
    virtual void transform() override;

    // Render the actors
    void render() override;
};

#endif
