#ifndef SEDNAS_HXX__
#define SEDNAS_HXX__

/**
 * @file SEDNAS.h++
 *
 * This is the main header for the SEDNAS project. It includes
 * the base headers for the project.
 *
 * @author Sean Salmon
 *
 * Created on: Dec 28, 2008
 */

/* STL headers that most classes will use */

#include <cassert>
#include <deque>
#include <iosfwd>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

using std::string;

typedef int64_t SizeType;
typedef int64_t TimeType;

namespace Time
{
    const TimeType NANOSECOND	= 1;
    const TimeType MICROSECOND	= NANOSECOND  * 1000;
    const TimeType MILLISECOND	= MICROSECOND * 1000;
    const TimeType SECOND	= MILLISECOND * 1000;
}

namespace Bandwidth
{
    const SizeType BIT		= 1;
    const SizeType KILOBIT	= BIT     * 1000;
    const SizeType MEGABIT	= KILOBIT * 1000;
    const SizeType GIGABIT	= MEGABIT * 1000;
}

namespace Size
{
    const SizeType BIT		= Bandwidth::BIT;    
    const SizeType BYTE		= BIT		* 8;
    const SizeType KILOBYTE	= BYTE		* 1024;
    const SizeType MEGABYTE	= KILOBYTE	* 1024;
    const SizeType GIGABYTE	= MEGABYTE	* 1024;
}

std::string
TimeToString(TimeType t);

std::string
SizeToString(SizeType s);

std::string
BandwidthToString(SizeType b);

TimeType
StringToTime(std::string s);

SizeType
StringToSize(std::string s);

SizeType
StringToBandwidth(std::string s);    

#endif /* SEDNAS_HXX__ */
