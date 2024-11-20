#include <stdio.h>
#include <curl/curl.h>
#include "email.h" // Include the header file for the function prototype

void sendEmailNotification(const char *subject, const char *body, const char *recipientEmail) {
    CURL *curl;
    CURLcode res;

    // Initialize libcurl
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing CURL\n");
        return;
    }

    // Set the email server details
    curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

    // Set the username and password for SMTP authentication
    curl_easy_setopt(curl, CURLOPT_USERNAME, "the.forecast.friend@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "iuuw rmoz fbfi kmiv");  // Replace with your App Password

    // Set the authentication method
    curl_easy_setopt(curl, CURLOPT_LOGIN_OPTIONS, "AUTH=PLAIN");

    // Create the MIME structure for the email
    struct curl_mime *mime = curl_mime_init(curl);

    // Set the "From" header
    struct curl_mimepart *from_part = curl_mime_addpart(mime);
    curl_mime_data(from_part, "From: the.forecast.friend@gmail.com\r\n", CURL_ZERO_TERMINATED);

    // Set the "To" header (recipient)
    struct curl_mimepart *to_part = curl_mime_addpart(mime);
    char to_header[256];
    snprintf(to_header, sizeof(to_header), "To: %s\r\n", recipientEmail);
    curl_mime_data(to_part, to_header, CURL_ZERO_TERMINATED);

    // Set the "Subject" header
    struct curl_mimepart *subject_part = curl_mime_addpart(mime);
    char subjectHeader[256];
    snprintf(subjectHeader, sizeof(subjectHeader), "Subject: %s\r\n", subject);
    curl_mime_data(subject_part, subjectHeader, CURL_ZERO_TERMINATED);

    // **Add the Content-Type for HTML in the email body**
    struct curl_mimepart *body_part = curl_mime_addpart(mime);
    char content_type[256];
    snprintf(content_type, sizeof(content_type), "Content-Type: text/html; charset=UTF-8\r\n\r\n"); // <-- NEW LINE
    curl_mime_data(body_part, content_type, CURL_ZERO_TERMINATED); // <-- NEW LINE
    curl_mime_data(body_part, body, CURL_ZERO_TERMINATED);

    // Attach the MIME structure to the email
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    // Set the recipient list
    struct curl_slist *recipients = NULL;
    recipients = curl_slist_append(recipients, recipientEmail);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    // Perform the email sending
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        fprintf(stderr, "Couldn't send Email, Due to: %s\n", curl_easy_strerror(res));
    } else {
        printf("Email sent successfully!\n");
    }

    // Clean up
    curl_slist_free_all(recipients);
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
}
