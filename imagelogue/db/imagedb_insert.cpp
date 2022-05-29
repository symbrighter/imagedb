#include <string>
#include <iostream>
#include <sqlite3.h> 
#include <unistd.h>

struct imagedb_insert_ctx_t {
    sqlite3 *db = nullptr;    
    long long max_image_id = -1;
}; 

/***************************************************************************
 *                                                                         *
 *                            Static Functions                             *
 *                                                                         *
 ***************************************************************************/

static int insert_cb (void *data, int argc, char **argv, char **az_col_name)
{
   int i;
   for(i = 0; i < argc; i++)
      std::cout << az_col_name[i] << "=" << argv[i] << std::endl;
   return 0;
}

static int max_imageid_cb (void *data, int argc, char **argv, char **col_name)
{
    if (argc != 1) {
        errno = EINVAL;
        return -1;
    }
    if (!argv[0]) {
        errno = EINVAL;
        return -1;
    }
    imagedb_insert_ctx_t *ctx = static_cast<imagedb_insert_ctx_t *> (data);
    try {
        ctx->max_image_id = static_cast<long long> (std::stoll (argv[0]));
    } catch (std::exception &e) {
        errno = EINVAL;
        return -1;
    }
    return 0;
}

static void build_sql_cmd (long long image_id,
                           const char *path, std::string &cmd)
{
    cmd = "INSERT INTO Images (ImageId, ImagePath) VALUES (";
    cmd += std::to_string (image_id) + std::string (", \'");
    cmd += path + std::string ("\'") + ")";
}


/***************************************************************************
 *                                                                         *
 *                            Public Functions                             *
 *                                                                         *
 ***************************************************************************/

extern "C" int imagedb_insert_image (sqlite3 *db, const char *path)
{
    if (!db || !path) {
        // TODO: Logging
        std::cerr << __FUNCTION__ << ": db || path" << std::endl;
        errno = EINVAL;
        return -1;
    }
    if (access (path, F_OK) != 0) {
        // TODO: Logging
        std::cerr << __FUNCTION__ << ": access" << std::endl;
        return -1;
    }

    imagedb_insert_ctx_t ctx;
    ctx.db = db;

    try {
        char *err_msg = 0;
        std::string cmd;
 
        if (sqlite3_exec (db, "SELECT MAX(ImageId) FROM Images;",
                          max_imageid_cb, &ctx, &err_msg) != SQLITE_OK) {
            // TODO: Handle logging 
            errno = EINVAL;
            return -1;
        }
        if (ctx.max_image_id < 0) {
            // TODO: Handle logging 
            errno = EINVAL;
            return -1;
        }
        build_sql_cmd (ctx.max_image_id + 1, path, cmd);
        if (sqlite3_exec (db, cmd.c_str (),
                          insert_cb, 0, &err_msg) != SQLITE_OK) {
            // TODO: Handle logging 
            errno = EINVAL;
            return -1;
        }
    } catch (std::bad_alloc &e) {
        // TODO: Handle logging 
        errno = ENOMEM;
        return -1;
    }
    return 0;
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
