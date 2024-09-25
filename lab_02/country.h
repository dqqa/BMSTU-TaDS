#ifndef COUNTRY_H_
#define COUNTRY_H_

#include <inttypes.h>
#include <stdbool.h>
#include "constants.h"

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
        /* EXCURSION */
        excursion_t type;
        uint32_t object_count;
        
        /* BEACH */
        season_t main_season;
        float air_temp;
        float water_temp;
        
        /* SPORT */
        sport_t sport;
    } tourism;
} country_t;

#endif // COUNTRY_H_
