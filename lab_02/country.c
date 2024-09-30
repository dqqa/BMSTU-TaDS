#include "country.h"
#include "errors.h"
#include "strlib.h"
#include <stdio.h>
#include <string.h>

static int get_tourism_info(FILE *fp, tourism_t *t)
{
    char buf[32];
    int rc;
    if ((rc = str_input(fp, buf, sizeof(buf))) != ERR_OK)
        return rc;

    if (strcmp(buf, "E") == 0)
        *t = TOURISM_EXCURSION;
    else if (strcmp(buf, "B") == 0)
        *t = TOURISM_BEACH;
    else if (strcmp(buf, "S") == 0)
        *t = TOURISM_SPORT;
    else
        return ERR_UNKNOWN_TYPE;

    return ERR_OK;
}

static int get_excursion_type(FILE *fp, excursion_t *e)
{
    char buf[32];
    int rc;
    if ((rc = str_input(fp, buf, sizeof(buf))) != ERR_OK)
        return rc;

    if (strcmp(buf, "N") == 0)
        *e = EXCURSION_NATURE;
    else if (strcmp(buf, "A") == 0)
        *e = EXCURSION_ART;
    else if (strcmp(buf, "H") == 0)
        *e = EXCURSION_HISTORY;
    else
        return ERR_UNKNOWN_TYPE;

    return ERR_OK;
}

static int get_beach_season(FILE *fp, season_t *s)
{
    char buf[32];
    int rc;
    if ((rc = str_input(fp, buf, sizeof(buf))) != ERR_OK)
        return rc;

    if (strcmp(buf, "winter") == 0)
        *s = SEASON_WINTER;
    else if (strcmp(buf, "spring") == 0)
        *s = SEASON_SPRING;
    else if (strcmp(buf, "summer") == 0)
        *s = SEASON_SUMMER;
    else if (strcmp(buf, "autumn") == 0)
        *s = SEASON_AUTUMN;
    else
        return ERR_UNKNOWN_TYPE;

    return ERR_OK;
}

static int get_sport_type(FILE *fp, sport_t *s)
{
    char buf[32];
    int rc;
    if ((rc = str_input(fp, buf, sizeof(buf))) != ERR_OK)
        return rc;

    if (strcmp(buf, "ski") == 0)
        *s = SPORT_SKIING;
    else if (strcmp(buf, "surf") == 0)
        *s = SPORT_SURFING;
    else if (strcmp(buf, "climb") == 0)
        *s = SPORT_CLIMBING;
    else
        return ERR_UNKNOWN_TYPE;

    return ERR_OK;
}

static int fill_tourism_info(FILE *fp, country_t *c)
{
    switch (c->main_tourism)
    {
        case TOURISM_EXCURSION: {
            if (fscanf(fp, "%" SCNu32, &c->tourism.exc_object_count) != 1)
                return ERR_IO;
            fgetc(fp);
            int rc = ERR_OK;
            if ((rc = get_excursion_type(fp, &(c->tourism.exc_type))) != ERR_OK)
                return rc;
        }
        break;
        case TOURISM_BEACH: {
            int rc = ERR_OK;
            if ((rc = get_beach_season(fp, &c->tourism.beach_main_season)) != ERR_OK)
                return rc;
            if (fscanf(fp, "%f", &c->tourism.beach_air_temp) != 1)
                return ERR_IO;
            fgetc(fp);
            if (fscanf(fp, "%f", &c->tourism.beach_water_temp) != 1)
                return ERR_IO;
            fgetc(fp);
        }
        break;
        case TOURISM_SPORT: {
            int rc = ERR_OK;
            if ((rc = get_sport_type(fp, &c->tourism.sport_type)) != ERR_OK)
                return rc;
        }
        break;
    }

    return ERR_OK;
}

int country_input(FILE *fp, country_t *country)
{
    int rc = ERR_OK;
    if ((rc = str_input(fp, country->name, sizeof(country->name))) != ERR_OK)
        return rc;
    if ((rc = str_input(fp, country->capital, sizeof(country->capital))) != ERR_OK)
        return rc;
    if ((rc = str_input(fp, country->continent, sizeof(country->continent))) != ERR_OK)
        return rc;

    int is_visa_needed = 0;
    if (fscanf(fp, "%d", &is_visa_needed) != 1)
        return ERR_IO;
    if (is_visa_needed != 0 && is_visa_needed != 1)
        return ERR_UNKNOWN_TYPE;
    country->is_visa_needed = is_visa_needed;
    fgetc(fp); // skip '/'

    if (fscanf(fp, "%" SCNu32, &country->travel_time) != 1)
        return ERR_IO;
    fgetc(fp); // skip '/'

    if (fscanf(fp, "%" SCNu32, &country->min_vacation_cost) != 1)
        return ERR_IO;
    fgetc(fp); // skip '/'

    if ((rc = get_tourism_info(fp, &country->main_tourism)) != ERR_OK)
        return rc;

    if ((rc = fill_tourism_info(fp, country)) != ERR_OK)
        return rc;

    return ERR_OK;
}

void country_print(FILE *fp, const country_t *c)
{
    fprintf(fp, "%s %s %s %d %" PRIu32 " %" PRIu32 " %d ", c->name, c->capital, c->continent, c->is_visa_needed, c->travel_time, c->min_vacation_cost, c->main_tourism);
    switch (c->main_tourism)
    {
        case TOURISM_EXCURSION:
            fprintf(fp, "%d %" PRIu32 "\n", c->tourism.exc_type, c->tourism.exc_object_count);
            break;
        case TOURISM_BEACH:
            fprintf(fp, "%d %.1f %.1f\n", c->tourism.beach_main_season, c->tourism.beach_air_temp, c->tourism.beach_water_temp);
            break;
        case TOURISM_SPORT:
            fprintf(fp, "%d\n", c->tourism.sport_type);
            break;
    }
}

int country_cmp_travel_time(const void *l, const void *r)
{
    return ((const country_t *)l)->travel_time > ((const country_t *)r)->travel_time;
}
