#include "/home/sean/SEDNAS/src/Logging/OStreamLogger.h++"

Logging::OStreamLogger
Logger("Test");

int main(int argc, char **argv)
{
    Logger.SuppressLogLevel(Logging::Warning);
    Logger << Logging::OStreamLogger::SetLogLevel<Logging::Fatal>;
    Logger << "Log Tester" << Logging::endl;
    Logger << "There are " << argc << " arguments:" << Logging::endl;
    Logger << Logging::Fatal << "This might be at a high enough log level..." << Logging::Fatal << Logging::endl;
    for(int index = 0; index < argc; ++index)
	Logger << index << ": " << argv[index] << Logging::endl;
    return 0;
}
