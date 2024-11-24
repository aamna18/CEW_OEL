#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "email.h"
#include "module1.h"

int main() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Unable to initialize CURL.\n");
        return 1;
    }

    FILE *fileptr = fopen("raw_api_data.json", "w");
    if (!fileptr) {
        fprintf(stderr, "Unable to open raw_api_data.json for writing.\n");
        curl_easy_cleanup(curl);
        return 1;
    }

    const char *api_url = "https://api.weatherapi.com/v1/forecast.json?key=0d02b9c3a02b45d4a91165418242110&q=Karachi&days=7";
    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fileptr);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Failed to perform CURL request: %s\n", curl_easy_strerror(res));
        fclose(fileptr);
        curl_easy_cleanup(curl);
        return 1;
    }

    curl_easy_cleanup(curl);
    fclose(fileptr);

    char *json_data = read_file("raw_api_data.json");
    if (!json_data) {
        fprintf(stderr, "Failed to read JSON data from file.\n");
        return 1;
    }

    printf("\n======================================\n");
    printf("        Temperature Information        \n");
    printf("======================================\n");
    calculate_and_print_average_temperatures(json_data);

    printf("\n======================================\n");
    printf("      Current Weather Information      \n");
    printf("======================================\n");
    get_current_weather(json_data);

    printf("\n======================================\n");
    printf("          Humidity Information         \n");
    printf("======================================\n");
    calculate_and_print_humidity(json_data);

    free(json_data);
    return 0;
}
