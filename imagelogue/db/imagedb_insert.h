#ifndef IMAGEDB_INSERT_H
#define IMAGEDB_INSERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sqlite3.h>

int imagedb_insert_image (sqlite3 *db, const char *path);

#ifdef __cplusplus
}
#endif

#endif // IMAGEDB_INSERT_H

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */

