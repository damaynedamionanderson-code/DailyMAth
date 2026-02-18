#ifndef EMAILSYSTEM_H
#define EMAILSYSTEM_H

#include <string>

class EmailSystem {
public:
    EmailSystem();
    ~EmailSystem();
    // This MUST have the 'int questionId' at the end to match your .cpp file
    bool sendUpdateNotification(const std::string& recipient, const std::string& difficulty, int questionId);

private:
    static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
};

#endif