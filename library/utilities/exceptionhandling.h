/************************************************************************
*    FILE NAME:       exceptionhandling.h
*
*    DESCRIPTION:     Exception handling classes
************************************************************************/           

#ifndef __exception_handling_h__
#define __exception_handling_h__

// Standard lib dependencies
#include <string>

namespace NExcept
{
    // Interface exception class
    class IException
    {
    public:
        
        IException( std::string title, std::string msg ) : errorTitle(title), errorMsg(msg)
        {}

        // Return the error info
        std::string & GetErrorMsg(){ return errorMsg; }
        std::string & GetErrorTitle(){ return errorTitle; }

    protected:

        std::string errorTitle;
        std::string errorMsg;
        
    };

    // Critical exception class. Used for non-recoverable exceptions
    class CCriticalException : public IException
    {
    public:
        
        CCriticalException( std::string title, std::string msg ) : IException(title, msg) {}
    };
}

#endif  // __exception_handling_h__