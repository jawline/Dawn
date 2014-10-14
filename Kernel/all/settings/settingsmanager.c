#include <settings/settingsmanager.h>
#include <stdlib.h>
#include <debug/debug.h>
#include <types/size_t.h>
#include <fs/vfs.h>

///TODO: Rewrite the line evaluator so its more robust

/*
 GLOBAL VARIABLES
 */

settingsEntry* settingsListStart = 0;

/*
 FUNCTION DEFINITIONS
 */

settingsEntry* settingsGetEntry(const char* name);
void settingsCreateEntry(const char* name, const char* data);
void parseConfigFile(const char* filePath);
void settingsModifyEntry(settingsEntry* entry, const char* newData);

/*
 PUBLIC FUNCTIONS
 */

/**
 * Initializes the settings manager
 * Loads base settings from /system/root/kernel.config
 */
void initializeSettingsManager() {

	DEBUG_PRINT("Initialized settings manager\n");
	settingsListStart = 0;
	parseConfigFile("/system/system.config");
	parseConfigFile("/system/kconf.config");
}

/**
 * Executes a line, like a query and returns any results there may be
 */

unsigned char settingsExecuteLine(const char* line) {

	size_t length = strlen(line);
	const char* firstSpace = strchr(line, ' ');

	if (firstSpace == 0) {
		DEBUG_PRINT("SETTINGS: ERROR INVALID COMMAND\n");
		return 0;
	}

	const char* secondSpace = strchr(firstSpace + 1, ' ');

	if (secondSpace == 0) {
		DEBUG_PRINT("SETTINGS: ERROR INVALID COMMAND\n");
		return 0;
	}

	const char* end = line + length;

	if (*(firstSpace + 1) != '=') {
		DEBUG_PRINT("SETTINGS: ERROR INVALID COMMAND\n");
		return 0;
	}

	if (firstSpace - line == 0) {
		DEBUG_PRINT("SETTINGS: ERROR INVALID COMMAND\n");
		return 0;
	}

	if (end - secondSpace == 0) {
		DEBUG_PRINT("SETTINGS: ERROR INVALID COMMAND\n");
		return 0;
	}

	char* Name = malloc(length + 2);
	memcpy(Name, line, firstSpace - line);
	Name[firstSpace - line] = '\0';

	char* Data = malloc(length + 2);
	memcpy(Data, secondSpace + 1, end - secondSpace - 1);
	Data[end - secondSpace - 1] = '\0';

	settingsEntry* oldEntry = settingsGetEntry(Name);

	if (oldEntry == 0) {
		//Create a new entry
		settingsCreateEntry(Name, Data);
	} else {
		//Reuse a old entry
		settingsModifyEntry(oldEntry, Data);
	}

	free(Name);
	free(Data);

	return 1;
}

/**
 * Reads the value of the specified settings entry
 **/

const char* settingsReadValue(char const* name, char const* defaultValue) {

	settingsEntry* entry = settingsGetEntry(name);
	return entry ? entry->data : defaultValue;
}

/*
 PRIVATE FUNCTIONS
 */

void parseConfigFile(const char* filePath) {

	DEBUG_PRINT("Parsing configuration file %s\n", filePath);

	fs_node_t* cfgNode = evaluatePath(filePath, init_vfs());

	if (cfgNode == 0)
		return;

	char* cfgBuffer = malloc(4096);
	memset(cfgBuffer, 0, 4096);

	size_t length = cfgNode->length;
	size_t current = 0;
	size_t iter = 0;

	open_fs(cfgNode);

	while (1) {

		if (current >= cfgNode->length) {
			break;
		}

		read_fs(cfgNode, current, 1, (uint8_t*) cfgBuffer + iter);

		if (*(cfgBuffer + iter) == '\n') {
			*(cfgBuffer + iter) = '\0';
			iter = 0;
			settingsExecuteLine(cfgBuffer);
			memset(cfgBuffer, 0, 4096);
		} else {
			iter++;
		}

		current++;
	}

	close_fs(cfgNode);

	free(cfgBuffer);
}

settingsEntry* settingsGetEntry(const char* name) {

	if (settingsListStart != 0) {

		settingsEntry* iter = settingsListStart;

		while (iter->next != 0) {

			if (strcmp(name, iter->name) == 0) {
				break;
			}

			iter = iter->next;
		}

		//Double check
		if (strcmp(name, iter->name) != 0) {
			return 0;
		}

		return iter;

	}

	return 0;
}

settingsEntry* settingsGetLastEntry() {

	if (settingsListStart != 0) {

		settingsEntry* iter = settingsListStart;

		while (iter->next != 0) {
			iter = iter->next;
		}

		return iter;
	}

	return 0;
}

void settingsModifyEntry(settingsEntry* entry, const char* newData) {

	if (entry->data != 0) {
		free(entry->data);
	}

	entry->data = malloc(strlen(newData) + 1);
	strcpy(entry->data, newData);
}

void settingsCreateEntry(const char* name, const char* data) {

	settingsEntry* entryCheck = settingsGetEntry(name);

	if (entryCheck != 0) {
		settingsModifyEntry(entryCheck, data);
		return;
	}

	//Allocate the settings entry
	settingsEntry* newEntry = malloc(sizeof(settingsEntry));
	memset(newEntry, 0, sizeof(settingsEntry));

	//Allocate memory for the settings entry name and data
	newEntry->name = malloc(strlen(name) + 1);
	newEntry->data = malloc(strlen(data) + 1);

	//Copy the name and data
	strcpy(newEntry->name, name);
	strcpy(newEntry->data, data);

	//Double check the next entry is pointed to null
	newEntry->next = 0;

	//Find the last entry
	settingsEntry* last = settingsGetLastEntry();

	if (last != 0) {
		//Tack this entry on to the end
		last->next = newEntry;
	} else {
		//Set settingsListStart to this new entry
		settingsListStart = newEntry;
	}
}