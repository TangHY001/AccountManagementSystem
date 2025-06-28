#define _CRT_SECURE_NO_WARNINGS
#include <time.h>

#include <stdio.h>

void timeToString(time_t t, char* pBuf) {
	struct tm* timeinfo = localtime(&t);
	if (timeinfo == NULL) {
		snprintf(pBuf, 20, "Invalid time");
		return;
	}
	strftime(pBuf, 20, "%Y-%m-%d %H:%M:%S", timeinfo);
}

time_t stringToTime(char* pTime) {
	struct tm timeinfo;
	time_t t;
	sscanf(pTime, "%d-%d-%d %d:%d:%d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday,
		&timeinfo.tm_hour, &timeinfo.tm_min, &timeinfo.tm_sec);
	timeinfo.tm_year -= 1900;
	timeinfo.tm_mon -= 1;
	t = mktime(&timeinfo);
	return t;
}