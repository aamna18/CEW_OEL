#include <stdio.h>
#include <curl/curl.h>
#include "email.h" 

void sendEmailNotification(const char *subject, const char *body, const char *recipientEmail) {
    CURL *curl;
    CURLcode res;

    
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing CURL\n");
        return;
    }

    
    curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

   
    curl_easy_setopt(curl, CURLOPT_USERNAME, "the.forecast.friend@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "iuuw rmoz fbfi kmiv"); 

   
    curl_easy_setopt(curl, CURLOPT_LOGIN_OPTIONS, "AUTH=PLAIN");

   
    struct curl_mime *mime = curl_mime_init(curl);

    
    struct curl_mimepart *from_part = curl_mime_addpart(mime);
    curl_mime_data(from_part, "From: the.forecast.friend@gmail.com\r\n", CURL_ZERO_TERMINATED);

    
    struct curl_mimepart *to_part = curl_mime_addpart(mime);
    char to_header[256];
    snprintf(to_header, sizeof(to_header), "To: %s\r\n", recipientEmail);
    curl_mime_data(to_part, to_header, CURL_ZERO_TERMINATED);

    
    struct curl_mimepart *subject_part = curl_mime_addpart(mime);
    char subjectHeader[256];
    snprintf(subjectHeader, sizeof(subjectHeader), "Subject: %s\r\n", subject);
    curl_mime_data(subject_part, subjectHeader, CURL_ZERO_TERMINATED);

    
    struct curl_mimepart *body_part = curl_mime_addpart(mime);
    char content_type[256];
    snprintf(content_type, sizeof(content_type), "Content-Type: text/html; charset=UTF-8\r\n\r\n"); 
    curl_mime_data(body_part, content_type, CURL_ZERO_TERMINATED); 
    curl_mime_data(body_part, body, CURL_ZERO_TERMINATED);

   
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

   
    struct curl_slist *recipients = NULL;
    recipients = curl_slist_append(recipients, recipientEmail);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    
    res = curl_easy_perform(curl);

    
    if (res != CURLE_OK) {
        fprintf(stderr, "Couldn't send Email, Due to: %s\n", curl_easy_strerror(res));
    } else {
        printf("Email sent successfully!\n");
    }

    
    curl_slist_free_all(recipients);
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
}
