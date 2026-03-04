#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    float cLow;
    float cHigh;
    int iLow;
    int iHigh;
} AQIBreakpoint;

typedef struct {
    float pm25;
    float pm10;
    float co;
    float so2;
    float no2;
    float o3;
    float temperature;
    float humidity;
} SensorData;

AQIBreakpoint pm25_breakpoints[] = {
    {0.0, 12.0, 0, 50},
    {12.1, 35.4, 51, 100},
    {35.5, 55.4, 101, 150},
    {55.5, 150.4, 151, 200},
    {150.5, 250.4, 201, 300},
    {250.5, 500.4, 301, 500}
};

AQIBreakpoint pm10_breakpoints[] = {
    {0, 54, 0, 50},
    {55, 154, 51, 100},
    {155, 254, 101, 150},
    {255, 354, 151, 200},
    {355, 424, 201, 300},
    {425, 604, 301, 500}
};

int calculateAQI(float concentration, AQIBreakpoint breakpoints[], int numBreakpoints) {
    for (int i = 0; i < numBreakpoints; i++) {
        if (concentration >= breakpoints[i].cLow && concentration <= breakpoints[i].cHigh) {
            float aqi = ((breakpoints[i].iHigh - breakpoints[i].iLow) /
                        (breakpoints[i].cHigh - breakpoints[i].cLow)) *
                        (concentration - breakpoints[i].cLow) + breakpoints[i].iLow;
            return (int)round(aqi);
        }
    }
    return -1;
}

int readSensorData(const char* filename, SensorData* data) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'. Please check the filename and try again.\n", filename);
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "PM2.5:%f", &data->pm25) == 1) continue;
        if (sscanf(line, "PM10:%f",  &data->pm10) == 1) continue;
        if (sscanf(line, "CO:%f",    &data->co)   == 1) continue;
        if (sscanf(line, "SO2:%f",   &data->so2)  == 1) continue;
        if (sscanf(line, "NO2:%f",   &data->no2)  == 1) continue;
        if (sscanf(line, "O3:%f",    &data->o3)   == 1) continue;
        if (sscanf(line, "TEMP:%f",  &data->temperature) == 1) continue;
        if (sscanf(line, "HUMIDITY:%f", &data->humidity) == 1) continue;
    }

    fclose(file);
    return 1;
}

void reflexAgent(SensorData* data, int* finalAQI, char* category, char* action) {
    int pm25_aqi = 0, pm10_aqi = 0;

    if (data->pm25 >= 0)
        pm25_aqi = calculateAQI(data->pm25, pm25_breakpoints, 6);
    if (data->pm10 >= 0)
        pm10_aqi = calculateAQI(data->pm10, pm10_breakpoints, 6);

    *finalAQI = (pm25_aqi > pm10_aqi) ? pm25_aqi : pm10_aqi;

    if (*finalAQI <= 50) {
        strcpy(category, "Good");
        strcpy(action, "Air quality is satisfactory. Enjoy outdoor activities!");
    } else if (*finalAQI <= 100) {
        strcpy(category, "Moderate");
        strcpy(action, "Air quality is acceptable. Sensitive individuals should limit prolonged outdoor exertion.");
    } else if (*finalAQI <= 150) {
        strcpy(category, "Unhealthy for Sensitive Groups");
        strcpy(action, "Sensitive groups should reduce outdoor activities. General public can continue normal activities.");
    } else if (*finalAQI <= 200) {
        strcpy(category, "Unhealthy");
        strcpy(action, "Everyone should reduce prolonged outdoor exertion. Use air purifiers indoors.");
    } else if (*finalAQI <= 300) {
        strcpy(category, "Very Unhealthy");
        strcpy(action, "Avoid outdoor activities. Keep windows closed and use air purifiers.");
    } else {
        strcpy(category, "Hazardous");
        strcpy(action, "Emergency conditions! Everyone should avoid all outdoor activities. Stay indoors with air filtration.");
    }

    if (data->temperature > 35 && *finalAQI > 100)
        strcat(action, " High temperature alert - stay hydrated!");
    if (data->humidity > 80 && *finalAQI > 100)
        strcat(action, " High humidity may worsen air quality perception.");
}

void displayResults(SensorData* data, int aqi, const char* category, const char* action) {
    printf("\n+====================================================+\n");
    printf("|         AIR QUALITY MONITORING SYSTEM              |\n");
    printf("|              Simple Reflex Agent                   |\n");
    printf("+====================================================+\n\n");

    printf("SENSOR READINGS:\n");
    printf("- PM2.5:       %.1f ug/m3\n", data->pm25);
    printf("- PM10:        %.1f ug/m3\n", data->pm10);
    printf("- CO:          %.2f ppm\n",   data->co);
    printf("- SO2:         %.1f ppb\n",   data->so2);
    printf("- NO2:         %.1f ppb\n",   data->no2);
    printf("- O3:          %.3f ppm\n",   data->o3);
    printf("- Temperature: %.1f C\n",     data->temperature);
    printf("- Humidity:    %.1f%%\n",     data->humidity);

    printf("\nAQI ASSESSMENT:\n");
    printf("- AQI Value: %d\n", aqi);
    printf("- Category:  %s\n", category);

    printf("\nAQI SCALE:\n");
    if      (aqi <= 50)  printf("  [GOOD] <- You are here\n");
    else if (aqi <= 100) printf("  [MODERATE] <- You are here\n");
    else if (aqi <= 150) printf("  [UNHEALTHY FOR SENSITIVE] <- You are here\n");
    else if (aqi <= 200) printf("  [UNHEALTHY] <- You are here\n");
    else if (aqi <= 300) printf("  [VERY UNHEALTHY] <- You are here\n");
    else                 printf("  [HAZARDOUS] <- You are here\n");

    printf("\nRECOMMENDED ACTION:\n   %s\n", action);
    printf("\n=====================================================\n");
}

int main() {
    SensorData sensorData;
    int aqi = 0;
    char category[50];
    char action[600];
    char filename[100];
    char choice;

    printf("=====================================================\n");
    printf("    Welcome to AQI Monitoring Reflex Agent System\n");
    printf("=====================================================\n");

printf("\nIMPORTANT INSTRUCTIONS:\n");
    printf("Provide a .txt file with sensor data in the following format:\n\n");
    printf("PM2.5:<value>\n");
    printf("PM10:<value>\n");
    printf("CO:<value>\n");
    printf("SO2:<value>\n");
    printf("NO2:<value>\n");
    printf("O3:<value>\n");
    printf("TEMP:<value>\n");
    printf("HUMIDITY:<value>\n\n");
    do {
        memset(&sensorData, 0, sizeof(SensorData));

        printf("\nEnter sensor data filename: ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';

        if (!readSensorData(filename, &sensorData)) {
            printf("Please try again.\n");
            /* consume leftover newline before looping back */
            printf("\nMonitor again? (y/n): ");
            scanf(" %c", &choice);
            while(getchar() != '\n');
            continue;
        }

        reflexAgent(&sensorData, &aqi, category, action);
        displayResults(&sensorData, aqi, category, action);

        printf("\nMonitor again? (y/n): ");
        scanf(" %c", &choice);
        while(getchar() != '\n'); /* flush for next fgets */

    } while (choice == 'y' || choice == 'Y');

    printf("\nExiting AQI Monitor. Stay safe!\n");
    return 0;
}
