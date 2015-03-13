#ifndef _SETTINGS_MANAGER_DEF_H_
#define _SETTINGS_MANAGER_DEF_H_

struct settings_manager_entry {
	char* name;
	char* data;
	struct settings_manager_entry* next;
};

typedef struct settings_manager_entry settingsEntry;

/**
 * Initializes the settings manager
 * Loads base settings from /system/root/kernel.config
 */
void initializeSettingsManager();

/**
 * Executes a line, like a query and returns any results there may be
 */
unsigned char settingsExecuteLine(char const* line);

/**
 * Reads the value of the specified settings entry
 */
const char* settingsReadValue(char const* name, char const* defaultValue);

#endif //_SETTINGS_MANAGER_DEF_H_
