#ifndef HOMEKIT_APPLICATIONS_SYSTEM_TIME_H_
#define HOMEKIT_APPLICATIONS_SYSTEM_TIME_H_

#include <time.h>

#include <NTPClient.h>

class SystemTime : public NTPClient
{
private:
public:
    using NTPClient::NTPClient;
    String getFormattedDateTime() const;
};

#endif // HOMEKIT_APPLICATION_SYSTEM_TIME_H_
