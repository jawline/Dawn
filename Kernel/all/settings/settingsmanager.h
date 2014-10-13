#ifndef _SETTINGS_MANAGER_DEF_H_
#define _SETTINGS_MANAGER_DEF_H_

struct settings_manager_entry
{
	char* Name;
	char* Data;

	struct settings_manager_entry* next;
};

typedef struct settings_manager_entry settingsEntry;

void initializeSettingsManager();
const char* settingsExecuteLine(const char* Line);
const char* settingsReadValue(const char* Name);

#endif //_SETTINGS_MANAGER_DEF_H_
