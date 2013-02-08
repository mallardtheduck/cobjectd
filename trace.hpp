/*!
    \file trace.hpp
    A debugging trace facility
*/

#ifndef TRACE_HPP_INCLUDED
#define TRACE_HPP_INCLUDED

#include <iostream>
#include <sstream>
#include <string>

/*!
    \macro NTRACE
    If defined, tracing is disabled.
*/

#ifdef NDEBUG
#define NTRACE
#endif

#ifndef NTRACE

/*!
    Enable tracing for this function/method
*/
#define TRACE static int trace_callid=0; trace_callid++;\
    do{if(TraceToggleCount==0) std::clog << __PRETTY_FUNCTION__ << ":" << trace_callid << ": START" << std::endl;}while(false);\
    std::stringstream _endmsgss;\
    _endmsgss << __PRETTY_FUNCTION__ << ":" << trace_callid << ": END"; \
    clog_at_end _cae(_endmsgss.str())

/*!
    Trace EXPression; add an expression and its result to the trace
*/
#define TEXP(x) do{if(TraceToggleCount==0) std::clog << __PRETTY_FUNCTION__ << ":" << trace_callid << ":" << #x << ":" << x << std::endl;}while(false)
/*!
    Trace COMent; add an arbitary string to the trace
*/
#define TCOM(x) do{if(TraceToggleCount==0) std::clog << __PRETTY_FUNCTION__ << ":" << trace_callid << ":" << x << std::endl;}while(false)
/*!
    Turn tracing off temporarily
*/
#define TOFF TCOM("TRACE OFF"); TraceToggleCount++
/*!
    Turn tracing back on
*/
#define TON TraceToggleCount--; TCOM("TRACE ON")
/*!
    Turn off tracing for the lifetime of the current scope
*/
#define TOFFSCOPE toff_scope _tos

#else

//Turn the trace macros into no-ops if tracing is disabled
#define TRACE (void)0
#define TEXP(x) (void)0
#define TCOM(x) (void)0
#define TOFF (void)0
#define TON (void)0
#define TOFFSCOPE (void)0

#endif

extern int TraceToggleCount; //!< The number of times that tracing has been turned off. Every TOFF must be matched by a TON.

/*!
    Output a trace message when this object goes out of scope. Relys on C++'s deterministic destruction of local variables.
    \note If an error occurs in another object's destructor, then it could appear after the end message
*/
class clog_at_end
{
private:
    std::string _msg;   //!< The message to output
public:
    /*!
        Constructor
    */
    clog_at_end(std::string msg) : _msg(msg){}

    /*!
        Destructor
    */
    ~clog_at_end()
    {
        if (TraceToggleCount==0) std::clog << _msg << std::endl;
    }
};

/*!
    Turn off tracing for the lifetime of this object. When used as a local, this turns off tracing for a scope.
*/
class toff_scope
{
public:
    /*!
        Constructor - turn off tracing
    */
    toff_scope()
    {
        TRACE;
        TOFF;
    }
    /*!
        Destructor - turn on tracing
    */
    ~toff_scope()
    {
        TRACE;
        TON;
    }
};


#endif // TRACE_HPP_INCLUDED