#include "datetime.h"
#include <string.h>
#include <assert.h>
#include <new>

using namespace std;
using namespace std::chrono;

BEGIN_AS_NAMESPACE

// TODO: Allow setting the timezone to use

// TODO: Allow taking the difference of two CDateTimes. Should return the difference in seconds
// TODO: Allow adding seconds
// TODO: Allow setting each component year, month, day, hour, minute, second. 
//       SetDate(y,m,d): year, month and day must be set in a single function to make sure the function can validate the date
//       SetTime(h,m,s): hour, minute and second shall be set in a single function too for consistency

static tm time_point_to_tm(const std::chrono::time_point<std::chrono::system_clock> &tp)
{
	time_t t = system_clock::to_time_t(tp);
	tm local;
#ifdef _MSC_VER
	localtime_s(&local, &t);
#else
	local = *localtime(&t);
#endif
	return local;
}

CDateTime::CDateTime() : tp(std::chrono::system_clock::now()) 
{
}

CDateTime::CDateTime(const CDateTime &o) : tp(o.tp) 
{
}

CDateTime &CDateTime::operator=(const CDateTime &o)
{
	tp = o.tp;
	return *this;
}

asUINT CDateTime::getYear() const
{
	tm local = time_point_to_tm(tp);
	return local.tm_year + 1900;
}

asUINT CDateTime::getMonth() const
{
	tm local = time_point_to_tm(tp);
	return local.tm_mon + 1;
}

asUINT CDateTime::getDay() const
{
	tm local = time_point_to_tm(tp);
	return local.tm_mday;
}

asUINT CDateTime::getHour() const
{
	tm local = time_point_to_tm(tp);
	return local.tm_hour;
}

asUINT CDateTime::getMinute() const
{
	tm local = time_point_to_tm(tp);
	return local.tm_min;
}

asUINT CDateTime::getSecond() const
{
	tm local = time_point_to_tm(tp);
	return local.tm_sec;
}

static void Construct(CDateTime *mem)
{
	new(mem) CDateTime();
}

static void ConstructCopy(CDateTime *mem, const CDateTime &o)
{
	new(mem) CDateTime(o);
}

void RegisterScriptDateTime(asIScriptEngine *engine)
{
	// TODO: Add support for generic calling convention
	assert(strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0);

	int r = engine->RegisterObjectType("datetime", sizeof(CDateTime), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<CDateTime>()); assert(r >= 0);

	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Construct), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_CONSTRUCT, "void f(const datetime &in)", asFUNCTION(ConstructCopy), asCALL_CDECL_OBJLAST); assert(r >= 0);
	r = engine->RegisterObjectMethod("datetime", "datetime &opAssign(const datetime &in)", asMETHOD(CDateTime, operator=), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("datetime", "uint get_year() const", asMETHOD(CDateTime, getYear), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("datetime", "uint get_month() const", asMETHOD(CDateTime, getMonth), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("datetime", "uint get_day() const", asMETHOD(CDateTime, getDay), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("datetime", "uint get_hour() const", asMETHOD(CDateTime, getHour), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("datetime", "uint get_minute() const", asMETHOD(CDateTime, getMinute), asCALL_THISCALL); assert(r >= 0);
	r = engine->RegisterObjectMethod("datetime", "uint get_second() const", asMETHOD(CDateTime, getSecond), asCALL_THISCALL); assert(r >= 0);
}

END_AS_NAMESPACE