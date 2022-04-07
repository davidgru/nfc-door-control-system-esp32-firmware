#pragma once


#ifndef WIFI_SSID
#error Specify WIFI_SSID in compiler flags
#endif
#ifndef WIFI_PASSWORD
#error Specify WIFI_PASSWORD in compiler flags
#endif


#ifndef PSKEY
#error Specify PSKEY in compiler flags
#endif

#ifndef TOKEN_SEED
#error Specify TOKEN_SEED in compiler flags
#endif


#if defined(DC_USE_CACHED_DATABASE)
#define DC_UNDERLYING_DATABASE DC::CachedDatabase
#elif defined(DC_USE_SORTED_DATABASE)
#define DC_UNDERLYING_DATABASE DC::SortedDatabase
#else
#error Choose the database in compiler flags
#endif

#define DATABASE_FILE "/tokens"
