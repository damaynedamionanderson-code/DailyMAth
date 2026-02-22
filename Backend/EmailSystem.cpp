#include "EmailSystem.h"
#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include <string>

// --- FIX: This struct must be defined HERE (at the top), not inside a function ---
struct WriteThis {
    const char *readptr;
    size_t sizeleft;
};

// Constructor
EmailSystem::EmailSystem() { 
    curl_global_init(CURL_GLOBAL_ALL); 
}

// Destructor
EmailSystem::~EmailSystem() { 
    curl_global_cleanup(); 
}

// Callback function used by CURL to send data
size_t EmailSystem::payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    struct WriteThis *upload = (struct WriteThis *)userp;
    size_t len = size * nmemb;

    if(upload->sizeleft) {
        size_t copy = (upload->sizeleft < len) ? upload->sizeleft : len;
        memcpy(ptr, upload->readptr, copy);
        upload->readptr += copy;
        upload->sizeleft -= copy;
        return copy;
    }
    return 0;
}

// Main function to send the email
bool EmailSystem::sendUpdateNotification(const std::string& recipient, const std::string& difficulty, int questionId) {
    CURL *curl = curl_easy_init();
    if(!curl) return false;

    // --- CREDENTIALS ---
    const char* from = "dailymmda@gmail.com";
    const char* password = "jhtj ndjx cufz rzcc"; // Your App Password
    
    // --- LINKS ---
    std::string appLink = "https://daily-math.org/"; 
    std::string discordLink = "https://discord.gg/kW5f4cbyU"; 

    // --- EMAIL CONTENT (With Emojis) ---
    std::string payload_text = 
        "To: " + recipient + "\r\n" +
        "From: Daily Math App <" + from + ">\r\n" +
        "Subject: 🧠 New Math Problems are Live!\r\n" + 
        "Content-Type: text/plain; charset=UTF-8\r\n" + 
        "\r\n" + 
        "Hello Math Solver,\r\n\r\n" +
        "Fresh challenges have been uploaded to the database! Today's set is ready for you.\r\n\r\n" +
        "--------------------------------------------------\r\n" +
        " ✅ LINEUP: Easy, Medium, and Hard Problems\r\n" +
        "--------------------------------------------------\r\n\r\n" +
        "👉 SOLVE IN APP: " + appLink + "\r\n\r\n" +
        "Want to go over the solutions with the community?\r\n" +
        "💬 JOIN OUR DISCORD: " + discordLink + "\r\n\r\n" +
        "See you there,\r\n" +
        "The Daily Math App Team"; 
    // --- UPLOAD STRUCTURE ---
    // This works now because 'WriteThis' is defined at the top of the file!
    struct WriteThis upload = { payload_text.c_str(), payload_text.length() };

    // --- CURL SETUP ---
    curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
    curl_easy_setopt(curl, CURLOPT_USERNAME, from);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
    
    // Recipients List
    struct curl_slist *recipients = NULL;
    recipients = curl_slist_append(recipients, recipient.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    
    // Timeout (prevents hanging)
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); 

    // --- SEND ---
    CURLcode res = curl_easy_perform(curl);
    
    // --- CLEANUP ---
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}