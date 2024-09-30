#include "country.h"
#include "errors.h"
#include "strlib.h"
#include <stdio.h>
#include <string.h>

#define ARR_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef struct
{
    int val;
    const char *str;
} accordance_t;

accordance_t tourisms[] = {
    {TOURISM_EXCURSION, "E"},
    {TOURISM_BEACH, "B"},
    {TOURISM_SPORT, "S"}};

accordance_t excursions[] = {
    {EXCURSION_NATURE, "N"},
    {EXCURSION_ART, "A"},
    {EXCURSION_HISTORY, "H"}};

accordance_t seasons[] = {
    {SEASON_AUTUMN, "autumn"},
    {SEASON_SPRING, "spring"},
    {SEASON_SUMMER, "summer"},
    {SEASON_WINTER, "winter"}};

accordance_t sports[] = {
    {SPORT_SKIING, "ski"},
    {SPORT_CLIMBING, "climb"},
    {SPORT_SURFING, "surf"}};

const char *set_tourism_info(tourism_t t)
{
    for (size_t i = 0; i < ARR_SIZE(tourisms); i++)
    {
        if (t == tourisms[i].val)
        {
            return tourisms[i].str;
        }
    }
    return NULL;
}

const char *set_excursion_info(excursion_t t)
{
    for (size_t i = 0; i < ARR_SIZE(excursions); i++)
    {
        if (t == excursions[i].val)
        {
            return excursions[i].str;
        }
    }
    return NULL;
}

const char *set_sport_info(sport_t t)
{
    for (size_t i = 0; i < ARR_SIZE(sports); i++)
    {
        if (t == sports[i].val)
        {
            return sports[i].str;
        }
    }
    return NULL;
}

const char *set_season_info(season_t t)
{
    for (size_t i = 0; i < ARR_SIZE(seasons); i++)
    {
        if (t == seasons[i].val)
        {
            return seasons[i].str;
        }
    }
    return NULL;
}

static int get_tourism_info(FILE *fp, tourism_t *t)
{
    char buf[32];
    int rc;
    if ((rc = str_input(fp, buf, sizeof(buf))) != ERR_OK)
        return rc;
    for (size_t i = 0; i < ARR_SIZE(tourisms); i++)
    {
        if (strcmp(tourisms[i].str, buf) == 0)
        {
            *t = tourisms[i].val;
            return ERR_OK;
        }
    }

    return ERR_UNKNOWN_TYPE;
}

static int get_excursion_type(FILE *fp, excursion_t *e)
{
    char buf[32];
    int rc;
    if ((rc = str_input(fp, buf, sizeof(buf))) != ERR_OK)
        return rc;

    for (size_t i = 0; i < ARR_SIZE(excursions); i++)
    {
        if (strcmp(excursions[i].str, buf) == 0)
        {
            *e = excursions[i].val;
            return ERR_OK;
        }
    }

    return ERR_UNKNOWN_TYPE;
}

static int get_beach_season(FILE *fp, season_t *s)
{
    char buf[32];
    int rc;
    if ((rc = str_input(fp, buf, sizeof(buf))) != ERR_OK)
        return rc;
    for (size_t i = 0; i < ARR_SIZE(seasons); i++)
    {
        if (strcmp(seasons[i].str, buf) == 0)
        {
            *s = seasons[i].val;
            return ERR_OK;
        }
    }

    return ERR_UNKNOWN_TYPE;
}

static int get_sport_type(FILE *fp, sport_t *s)
{
    char buf[32];
    int rc;
    if ((rc = str_input(fp, buf, sizeof(buf))) != ERR_OK)
        return rc;
    for (size_t i = 0; i < ARR_SIZE(sports); i++)
    {
        if (strcmp(sports[i].str, buf) == 0)
        {
            *s = sports[i].val;
            return ERR_OK;
        }
    }

    return ERR_UNKNOWN_TYPE;
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
    fprintf(fp, "%s %s %s %d %" PRIu32 " %" PRIu32 " %s ", c->name, c->capital, c->continent, c->is_visa_needed, c->travel_time, c->min_vacation_cost, set_tourism_info(c->main_tourism));
    switch (c->main_tourism)
    {
        case TOURISM_EXCURSION:
            fprintf(fp, "%" PRIu32 " %s\n", c->tourism.exc_object_count, set_excursion_info(c->tourism.exc_type));
            break;
        case TOURISM_BEACH:
            fprintf(fp, "%s %.1f %.1f\n", set_season_info(c->tourism.beach_main_season), c->tourism.beach_air_temp, c->tourism.beach_water_temp);
            break;
        case TOURISM_SPORT:
            fprintf(fp, "%s\n", set_sport_info(c->tourism.sport_type));
            break;
    }
}

int country_cmp_travel_time(const void *l, const void *r)
{
    return ((const country_t *)l)->travel_time > ((const country_t *)r)->travel_time;
}
