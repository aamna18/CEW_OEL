#ifndef MODULE1_H
#define MODULE1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

void sendEmailNotification(const char *subject, const char *body, const char *recipientEmail);
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);
char* read_file(const char *filename);
void calculate_and_print_average_temperatures(const char *json_data);
void get_current_weather(const char *json_data);
void calculate_and_print_humidity(const char *json_data);

#endif // MODULE1_H
