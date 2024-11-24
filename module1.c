#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "email.h"

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    FILE *fp = (FILE *)userp;
    return fwrite(contents, size, nmemb, fp);
}

char* read_file(const char *filename) {
    FILE *fileptr = fopen(filename, "r");
    if (!fileptr) {
        printf("Could not open file: %s\n", filename);
        return NULL;
    }
    fseek(fileptr, 0, SEEK_END);
    long size_of_data = ftell(fileptr);
    fseek(fileptr, 0, SEEK_SET);

    char *json_data = (char *)malloc(size_of_data + 1);
    if (!json_data) {
        printf("Could not allocate memory\n");
        fclose(fileptr);
        return NULL;
    }

    size_t read_size = fread(json_data, 1, size_of_data, fileptr);
    json_data[read_size] = '\0';
    fclose(fileptr);
    return json_data;
}

// Function to calculate and display average temperatures, checking for temperature > 30°C
void calculate_and_print_average_temperatures(const char *json_data) {
    FILE *output_file = fopen("processed_data.txt", "w");
    struct json_object *parsed_json, *forecast, *forecastday, *hourly_data, *temp_c, *max_temp, *min_temp;
    double total_temp = 0.0, total_max_temp = 0.0, total_min_temp = 0.0;
    int hourly_count = 0, total_days = 0;

    parsed_json = json_tokener_parse(json_data);
    if (!parsed_json) {
        fprintf(stderr, "Error parsing JSON data.\n");
        return;
    }

    if (json_object_object_get_ex(parsed_json, "forecast", &forecast) &&
        json_object_object_get_ex(forecast, "forecastday", &forecastday)) {

        // Calculate today's average temperature from hourly data
        struct json_object *first_day_forecast = json_object_array_get_idx(forecastday, 0);
        if (json_object_object_get_ex(first_day_forecast, "hour", &hourly_data)) {
            for (int i = 0; i < json_object_array_length(hourly_data); i++) {
                struct json_object *hour_data = json_object_array_get_idx(hourly_data, i);
                if (json_object_object_get_ex(hour_data, "temp_c", &temp_c)) {
                    total_temp += json_object_get_double(temp_c);
                    hourly_count++;
                }
            }
        }

        // Calculate average max and min temperatures for the next 7 days
        int num_days = json_object_array_length(forecastday);
        for (int day = 0; day < num_days; day++) {
            struct json_object *day_forecast = json_object_array_get_idx(forecastday, day);
            struct json_object *day_info;
            if (json_object_object_get_ex(day_forecast, "day", &day_info)) {
                json_object_object_get_ex(day_info, "maxtemp_c", &max_temp);
                json_object_object_get_ex(day_info, "mintemp_c", &min_temp);
                total_max_temp += json_object_get_double(max_temp);
                total_min_temp += json_object_get_double(min_temp);
                total_days++;
            }
        }

        double daily_average_temp = (hourly_count > 0) ? (total_temp / hourly_count) : 0.0;
        double weekly_average_max_temp = (total_days > 0) ? (total_max_temp / total_days) : 0.0;
        double weekly_average_min_temp = (total_days > 0) ? (total_min_temp / total_days) : 0.0;

        printf("\n--- Temperature Information ---\n");
        printf("+--------------------------------------+--------+\n");
        printf("| Description                          | Value  |\n");
        printf("+--------------------------------------+--------+\n");
        printf("| Today's Average Temperature          | %.2f°C |\n", daily_average_temp);
        printf("| Average Maximum Temperature (7 Days) | %.2f°C |\n", weekly_average_max_temp);
        printf("| Average Minimum Temperature (7 Days) | %.2f°C |\n", weekly_average_min_temp);
        printf("+--------------------------------------+--------+\n");

        // Store the data in file

        fprintf(output_file, "Today's Average Temperature: %.2f°C\n", daily_average_temp);
        fprintf(output_file, "Average Maximum Temperature of Next 3 Days: %.2f°C\n", weekly_average_max_temp);
        fprintf(output_file, "Average Minimum Temperature of Next 3 Days: %.2f°C\n", weekly_average_min_temp);    

    } else {
        fprintf(stderr, "No forecast data found.\n");
    }

    json_object_put(parsed_json);
}

// Function to display current weather details
void get_current_weather(const char *json_data) {
    FILE *output_file = fopen("processed_data.txt", "a");
    struct json_object *parsed_json, *current, *temp_c, *wind_speed, *last_updated, *condition, *feels_like;

    parsed_json = json_tokener_parse(json_data);
    if (!parsed_json) {
        fprintf(stderr, "Error parsing JSON data.\n");
        return;
    }

    if (json_object_object_get_ex(parsed_json, "current", &current)) {
        json_object_object_get_ex(current, "temp_c", &temp_c);
        json_object_object_get_ex(current, "wind_kph", &wind_speed);
        json_object_object_get_ex(current, "last_updated", &last_updated);
        json_object_object_get_ex(current, "condition", &condition);
        json_object_object_get_ex(current, "feelslike_c", &feels_like);

        const char *date_and_time = json_object_get_string(last_updated);
        char date[20], time[20];
        sscanf(date_and_time, "%19s %19s", date, time);

        const char *condition_text = json_object_get_string(json_object_object_get(condition, "text"));

        printf("\n--- Current Weather Information ---\n");
        printf("+----------------------+--------------------+\n");
        printf("| Date                 | %s               |\n", date);
        printf("| Time                 | %s               |\n", time);
        printf("| Temperature          | %.2f°C           |\n", json_object_get_double(temp_c));
        printf("| Feels Like           | %.2f°C           |\n", json_object_get_double(feels_like));
        printf("| Condition            | %s               |\n", condition_text);
        printf("| Wind Speed           | %.2f kph         |\n", json_object_get_double(wind_speed));
        printf("+----------------------+--------------------+\n");
       
        // Store it in file
        fprintf(output_file, "Current Date: %s\n", date);
        fprintf(output_file, "Current Time: %s\n", time);
        fprintf(output_file, " Current Temperature: %.2f°C\n", json_object_get_double(temp_c));
        fprintf(output_file, "Feels Like: %.2f°C\n", json_object_get_double(feels_like));
        fprintf(output_file, "Current Condition: %s\n", json_object_get_string(json_object_object_get(condition, "text")));
        fprintf(output_file, "Wind Speed: %.2f kph\n", json_object_get_double(wind_speed));        
        double current_temp = json_object_get_double(temp_c);
        if (current_temp > 25.0) {
            printf("\033[1;31m*** ALERT: Today's average temperature exceeds 25°C! ***\033[0m\n");
            const char *alert_subject = "Temperature Alert: High Temperature Detected";
            char alert_body[512];
            snprintf(alert_body, sizeof(alert_body),
             "Warning!"
             "The average temperature today is %.2f°C, "
             "which exceeds the threshold of 25°C."
             , current_temp);

            sendEmailNotification(alert_subject, alert_body, "ahmedamna122@gmail.com"); // Replace with your recipient email
            system("./email_notification_sender.sh");
        }
    } else {
        fprintf(stderr, "No current weather data found.\n");
    }

    json_object_put(parsed_json);
}

// Function to calculate and display humidity information
void calculate_and_print_humidity(const char *json_data) {
    FILE *output_file = fopen("processed_data.txt", "a");
    struct json_object *parsed_json, *current, *forecast, *forecastday, *day_forecast, *humidity;
    int current_humidity;
    double total_weekly_humidity = 0.0, total_daily_humidity = 0.0;
    int total_days = 0;

    parsed_json = json_tokener_parse(json_data);
    if (!parsed_json) {
        fprintf(stderr, "Error parsing JSON data.\n");
        return;
    }

    if (json_object_object_get_ex(parsed_json, "current", &current) &&
        json_object_object_get_ex(current, "humidity", &humidity)) {
        current_humidity = json_object_get_int(humidity);
    }

    if (json_object_object_get_ex(parsed_json, "forecast", &forecast) &&
        json_object_object_get_ex(forecast, "forecastday", &forecastday)) {

        int num_days = json_object_array_length(forecastday);
        for (int i = 0; i < num_days; i++) {
            struct json_object *day = json_object_array_get_idx(forecastday, i);
            if (json_object_object_get_ex(day, "day", &day_forecast) &&
                json_object_object_get_ex(day_forecast, "avghumidity", &humidity)) {

                double day_humidity = json_object_get_double(humidity);
                total_weekly_humidity += day_humidity;
                if (i == 0) {
                    total_daily_humidity = day_humidity;
                }
                total_days++;
            }
        }

        double avg_weekly_humidity = (total_days > 0) ? (total_weekly_humidity / total_days) : 0.0;

        printf("\n--- Humidity Information ---\n");
        printf("+--------------------------------------+--------+\n");
        printf("| Description                          | Value  |\n");
        printf("+--------------------------------------+--------+\n");
        printf("| Current Humidity                     | %d%%   |\n", current_humidity);
        printf("| Today's Average Humidity             | %.1f%% |\n", total_daily_humidity);
        printf("| Average Humidity for the Next 7 Days | %.1f%% |\n", avg_weekly_humidity);
        printf("+--------------------------------------+--------+\n");
        // Storing data in file
        fprintf(output_file, "Current Humidity: %d%%\n", current_humidity);
        fprintf(output_file, "Today's Average Humidity: %.1f%%\n", total_daily_humidity);
        fprintf(output_file, "Average Humidity for the Next 3 Days: %.1f%%\n", avg_weekly_humidity);
    } else {
        fprintf(stderr, "No forecast humidity data found.\n");
    }

    json_object_put(parsed_json);
}