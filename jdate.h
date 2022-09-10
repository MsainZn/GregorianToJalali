
#ifndef __JDATE_H__
#define __JDATE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
  int Year;
  int Month;
  int Day;
} JalaliDate;

int GetYear (const char* dateString);
int GetMonth(const char* dateString);
int GetDay  (const char* dateString);
JalaliDate GregorianToJalali(int g_y, int g_m, int g_d);
const char* jdate(const char* dateString);

///////////////////////////////////////////////////////////////////////////////

int GetYear(const char *dateString)
{
  char strYear[5];
  memcpy(strYear, dateString, 4);
  strYear[4] = '\0';
  int year = atoi(strYear);
  return year;
}

int GetMonth(const char *dateString)
{
  char strMonth[3];
  memcpy(strMonth, dateString + 5, 2);
  strMonth[2] = '\0';
  int month = atoi(strMonth);
  return month;
}

int GetDay(const char *dateString)
{
  char strDay[3];
  memcpy(strDay, dateString + 8, 2);
  strDay[2] = '\0';
  int day = atoi(strDay);
  return day;
}

JalaliDate GregorianToJalali(int g_y, int g_m, int g_d)
{
  int g_days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int j_days_in_month[] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};

  long int gy = g_y - 1600;
  long int gm = g_m - 1;
  long int gd = g_d - 1;
  long int g_day_no, j_day_no;
  long int i, j_np, jy, j, jm, jd;

  JalaliDate Date;

  g_day_no = 365 * gy;
  g_day_no += (gy + 3) / 4;
  g_day_no -= (gy + 99) / 100;
  g_day_no += (gy + 399) / 400;

  for (i = 0; i < gm; ++i)
    g_day_no += g_days_in_month[i];

  if (gm > 1 && ((gy % 4 == 0 && gy % 100 != 0) || (gy % 400 == 0)))
    // leap and after Feb
    g_day_no++;
  g_day_no += gd;

  j_day_no = g_day_no - 79;

  j_np = j_day_no / 12053;
  j_day_no = j_day_no % 12053;

  jy = 979 + 33 * j_np + 4 * (j_day_no / 1461);

  j_day_no %= 1461;

  if (j_day_no >= 366)
  {
    jy += (j_day_no - 1) / 365;
    j_day_no = (j_day_no - 1) % 365;
  }

  for (j = 0; j < 11 && j_day_no >= j_days_in_month[j]; ++j)
    j_day_no -= j_days_in_month[j];
  jm = j + 1;
  jd = j_day_no + 1;

  Date.Day = jd;
  Date.Month = jm;
  Date.Year = jy;

  return Date;
}

const char* jdate(const char* dateString)
{
  if (dateString == NULL)
  {
    char* strJalali = (char*) calloc(11, sizeof(char));
    time_t t = time(NULL);
    struct tm currDate = *localtime(&t);
    JalaliDate ymd = GregorianToJalali (currDate.tm_year + 1900, currDate.tm_mon + 1, currDate.tm_mday);
    sprintf(strJalali, "%04d/%02d/%02d", ymd.Year, ymd.Month, ymd.Day);
    strJalali[10] = '\0';
    return strJalali;
  }
  // Verify Format
  else if (strlen(dateString) == 10)
  {
    // Get Data
    char* strJalali = (char*) calloc(11, sizeof(char));
    int day   = GetDay  (dateString);
    int month = GetMonth(dateString);
    int year  = GetYear (dateString);
    // Convert
    JalaliDate ymd = GregorianToJalali (year, month, day);
    sprintf(strJalali, "%04d/%02d/%02d", ymd.Year, ymd.Month, ymd.Day);
    strJalali[10] = '\0';
    return strJalali;
  }
  else
  {
    fprintf(stderr, "Wrong Format. Please use the given format: YYYY.MM.DD\n");
    return NULL;
  }
  
}

#endif

