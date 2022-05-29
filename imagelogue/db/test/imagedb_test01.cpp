#include <sqlite3.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include "imagedb.h"
#include "tap.h"

static void create_tables (sqlite3 *db)
{
    int rc;
    std::string cmd;
    char *err_msg;

    cmd = "CREATE TABLE [Images] (";
    cmd += "[ImageId] INTEGER NOT NULL PRIMARY KEY, ";
    cmd += "[ImagePath] NVARCHAR(4096) NOT NULL);";

    rc = sqlite3_exec (db, cmd.c_str (), NULL, 0, &err_msg);
    ok (rc == 0, "sqlite_exec on %s", cmd.c_str ());

    cmd = "CREATE TABLE [Devices] (";
    cmd += "[DeviceId] INTEGER NOT NULL PRIMARY KEY, ";
    cmd += "[DeviceName] NVARCHAR(128) NOT NULL, ";
    cmd += "[MemberId] INTEGER NOT NULL);";

    rc = sqlite3_exec (db, cmd.c_str (), NULL, 0, &err_msg);
    ok (rc == 0, "sqlite_exec on %s", cmd.c_str ());

    cmd = "CREATE TABLE [FamilyMembers] (";
    cmd += "[MemberId] INTEGER NOT NULL PRIMARY KEY, ";
    cmd += "[MemberName] NVARCHAR(128) NOT NULL, ";
    cmd += "[MemberDOB] DATE NULL);";

    rc = sqlite3_exec (db, cmd.c_str (), NULL, 0, &err_msg);
    ok (rc == 0, "sqlite_exec on %s", cmd.c_str ());

    cmd = "CREATE TABLE [ImageMetadata] (";
    cmd += "[ImageMetaId] INTEGER NOT NULL PRIMARY KEY, ";
    cmd += "[ImageId] NVARCHAR(4096) NOT NULL, ";
    cmd += "[DeviceId] INTEGER NOT NULL);";

    rc = sqlite3_exec (db, cmd.c_str (), NULL, 0, &err_msg);
    ok (rc == 0, "sqlite_exec on %s", cmd.c_str ());

    return;
}

static void add_rows (sqlite3 *db)
{
    int rc;
    std::string cmd;
    char *err_msg;

    cmd = "INSERT INTO Images(ImageId, ImagePath) ";
    cmd += "VALUES(11, 'data/20220525/20220523_124354.jpg');";

    rc = sqlite3_exec (db, cmd.c_str (), NULL, 0, &err_msg);
    ok (rc == 0, "sqlite_exec on %s", cmd.c_str ());

    cmd = "INSERT INTO Images(ImageId, ImagePath) ";
    cmd += "VALUES(12, 'data/20220525/SmartSelect_20220525-143253_Chrome.jpg');";

    rc = sqlite3_exec (db, cmd.c_str (), NULL, 0, &err_msg);
    ok (rc == 0, "sqlite_exec on %s", cmd.c_str ());

    return;
}

static int insert_validate_cb (void *data, int argc, char **argv, char **col)
{
    long long *max = static_cast<long long *> (data);
    try {
        *max = std::stoll (argv[0]);
    } catch (std::exception &e) {
        errno = EINVAL;
        return -1;
    }
    return 0;
}

static void insert_image (sqlite3 *db)
{
    int rc;
    std::string cmd;
    long long count;
    char *err_msg;

    rc = imagedb_insert_image (db, "./image1.jpg");
    ok (rc == 0, "imagedb_insert_image return success" );

    cmd = "SELECT COUNT(ImageId) FROM Images;";
    rc = sqlite3_exec (db, cmd.c_str (),
                       insert_validate_cb, &count, &err_msg);
    ok (rc == 0, "sqlite_exec on %s", cmd.c_str ());
    ok (count == 3, "count == 4"); 

    return;
}


extern "C" int main (int argc, char*argv[])
{
    int rc;
    sqlite3 *db = nullptr;

    plan (10);

    rc = sqlite3_open (":memory:", &db);
    ok (rc == 0, "sqlite3_open succeeded");

    create_tables (db);

    add_rows (db);

    insert_image (db);

    sqlite3_close (db);

    done_testing ();

    return EXIT_SUCCESS;
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
