#ifndef PUMA_ERRORS
#define PUMA_ERRORS

ERR(PUMA_NOERR, "No error has occurred"),
ERR(PUMA_OSERROR, "The OS returned an error"),
ERR(PUMA_ERROR_SHORT_FILE, "The data file is missing data"),
ERR(PUMA_ERROR_INVALID_DATA, "The data in the file is invalid"),
ERR(PUMA_ERROR_OOB, "The X or Y axes from the data file are too large"),

#endif
