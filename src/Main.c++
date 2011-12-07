#include <sstream>
#include "SEDNAS.h++"
#include "Simulation/Timer.h++"

Timer
Timer::Main;

std::string
TimeToString(TimeType t)
{
  static std::ostringstream G_OSS;

  long double val = t;
  val /= Time::SECOND;
  G_OSS.str("");
  G_OSS.unsetf(std::ios::floatfield);
  G_OSS << val;
  return G_OSS.str();
}

std::string
SizeToString(SizeType s)
{
  static 
	std::ostringstream
	G_OSS;


  double val = s;
  G_OSS.str("");
  G_OSS.unsetf(std::ios::floatfield);
  if(s > Size::GIGABYTE)
  {
	val /= Size::GIGABYTE;
	G_OSS << val << "GB";
  }
  else if(s > Size::MEGABYTE)
  {
	val /= Size::MEGABYTE;
	G_OSS << val << "MB";
  }
  else if(s > Size::KILOBYTE)
  {
	val /= Size::KILOBYTE;
	G_OSS << val << "KB";
  }
  else
  {
	G_OSS << val << "B";
  }
  return G_OSS.str();
}

std::string
BandwidthToString(SizeType s)
{
  static 
	std::ostringstream
	G_OSS;


  double val = s;
  G_OSS.str("");
  G_OSS.unsetf(std::ios::floatfield);
  if(s > Bandwidth::GIGABIT)
  {
	val /= Bandwidth::GIGABIT;
	G_OSS << val << "Gbit";
  }
  else if(s > Bandwidth::MEGABIT)
  {
	val /= Bandwidth::MEGABIT;
	G_OSS << val << "Mbit";
  }
  else if(s > Bandwidth::KILOBIT)
  {
	val /= Bandwidth::KILOBIT;
	G_OSS << val << "Kbit";
  }
  else
  {
	G_OSS << val << "bit";
  }
  return G_OSS.str();
}

TimeType
StringToTime(std::string s)
{
  static
	std::istringstream
	G_ISS;

  TimeType val;
  G_ISS.str(s);
  G_ISS >> val;
  if(s.find("ns") != std::string::npos)
	val *= Time::NANOSECOND;
  else if(s.find("us") != std::string::npos)
	val *= Time::MICROSECOND;
  else if(s.find("ms") != std::string::npos)
	val *= Time::MILLISECOND;
  else 
	val *= Time::SECOND;
  return val;
}

SizeType
StringToSize(std::string s)
{
  static
	std::istringstream
	G_ISS;

  SizeType val;
  G_ISS.str(s);
  G_ISS >> val;
  if(s.find("GB") != std::string::npos)
	val *= Size::GIGABYTE;
  else if(s.find("MB") != std::string::npos)
	val *= Size::MEGABYTE;
  else if(s.find("KB") != std::string::npos)
	val *= Size::KILOBYTE;
  else 
	val *= Size::BYTE;
  return val;
}

SizeType
StringToBandwidth(std::string s)
{
  static
	std::istringstream
	G_ISS;

  SizeType val;
  G_ISS.str(s);
  G_ISS >> val;
  if(s.find("Gb") != std::string::npos)
	val *= Bandwidth::GIGABIT;
  else if(s.find("Mb") != std::string::npos)
	val *= Bandwidth::MEGABIT;
  else if(s.find("Kb") != std::string::npos)
	val *= Bandwidth::KILOBIT;
  else 
	val *= Bandwidth::BIT;
  return val;
}

