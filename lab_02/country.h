#ifndef COUNTRY_H_
#define COUNTRY_H_

#include "constants.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum
{
    TOURISM_EXCURSION,
    TOURISM_BEACH,
    TOURISM_SPORT
} tourism_t;

typedef enum
{
    EXCURSION_NATURE,
    EXCURSION_ART,
    EXCURSION_HISTORY
} excursion_t;

typedef enum
{
    SEASON_WINTER,
    SEASON_SPRING,
    SEASON_SUMMER,
    SEASON_AUTUMN
} season_t;

typedef enum
{
    SPORT_SKIING,
    SPORT_SURFING,
    SPORT_CLIMBING
} sport_t;

typedef enum
{
    FIELD_NAME,
    FIELD_CAPITAL,
    FIELD_CONTINENT,
    FIELD_VISA,
    FIELD_TRAVEL_TIME,
    FIELD_MIN_VACATION_COST
} field_t;

typedef struct
{
    char name[MAX_COUNTRY_NAME_LEN];
    char capital[MAX_COUNTRY_CAPITAL_LEN];
    char continent[MAX_COUNTRY_CONTINENT_LEN];

    bool is_visa_needed;
    uint32_t travel_time;
    uint32_t min_vacation_cost;
    tourism_t main_tourism;

    union
    {
        // TODO: C99 forbids anonymous structs/unions
        /* EXCURSION */
        struct
        {
            excursion_t exc_type;
            uint32_t exc_object_count;
        };

        /* BEACH */
        struct
        {
            season_t beach_main_season;
            float beach_air_temp;
            float beach_water_temp;
        };

        /* SPORT */
        struct
        {
            sport_t sport_type;
        };
    } tourism;
} country_t;

int country_input(FILE *fp, country_t *country);
void country_print(FILE *fp, const country_t *country);

/* COMPARISON */
int country_cmp_name(const void *l, const void *r);
int country_cmp_country(const void *l, const void *r);
int country_cmp_capital(const void *l, const void *r);

int country_cmp_travel_time(const void *l, const void *r);

#endif // COUNTRY_H_
