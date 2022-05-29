#include <iostream>
#include <memory>
#include <string>
#include <cerrno>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sqlite3.h>
#include <getopt.h>
#include "imagedb.h"
#include "imagedb.grpc.pb.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using imagedb::ImageDb;
using imagedb::ImageDbInsertReply;
using imagedb::ImageDbInsertRequest;

#define OPTIONS "T:Dh"
static const struct option longopts[] = {
    {"target",  required_argument, 0, 'T'},
    {"dbpath",  required_argument, 0, 'D'},
    {"help",    no_argument,       0, 'h'},
    { 0, 0, 0, 0 },
};


struct server_ctx_t {
    std::string dbpath = "./ImagesDB.db";
    std::string target = "0.0.0.0:50051";
};

static void usage (int code)
{
    std::cerr <<
"usage: imagedb-server [OPTIONS...]\n"
"\n"
"OPTIONS:\n"
"    -h, --help\n"
"            Display this usage information\n"
"\n"
"    -T, --target=IP:port\n"
"            IP:Port to listen on\n"
"            (default=0.0.0.0:50051)\n"
"\n"
"    -D, --dbpath=sqlite3-filepath\n"
"            SQLite3 database file to use\n"
"            (default=./ImagesDB.db)\n"
"\n";
    exit (code);
}

static void process_args (server_ctx_t *ctx, int argc, char *argv[])
{
    int ch = 0;

    while ((ch = getopt_long (argc, argv, OPTIONS, longopts, NULL)) != -1) {
        switch (ch) {
            case 'h': /* --help */
                usage (0);
                break;
            case 'T': /* --target */
                ctx->target = optarg;
                break;
            case 'D': /* --target */
                ctx->dbpath = optarg;
                break;
            default:
                usage (1);
                break;
        }
    }

    if (optind != argc)
        usage (1);
}

class ImageDbServiceImpl final : public ImageDb::Service {
public:
    Status insert (ServerContext *context,
                   const ImageDbInsertRequest *request,
                   ImageDbInsertReply *reply) override
    {
        if (access (dbpath.c_str (), F_OK) != 0) {
            std::string msg = std::string ("ImageDB: dbpath=") + dbpath
                                  + std::string ("(") + strerror (ENOENT)
                                  + std::string (")");
            reply->set_message (msg);
            return grpc::Status (grpc::StatusCode::NOT_FOUND, "");
        }

        std::string path = request->path ();
#if 0
        if (access (path.c_str (), F_OK) != 0) {
            std::string msg = std::string ("ImageDB: path=")
                                  + path + std::string ("(")
                                  + strerror (ENOENT) + std::string (")");
            reply->set_message (msg);
            return grpc::Status (grpc::StatusCode::NOT_FOUND, "");
        }
#endif

        sqlite3 *db = nullptr;
        if (sqlite3_open (dbpath.c_str (), &db) != 0) {
            std::string msg = std::string ("ImageDB: sqlite3_open (")
                                  + strerror (errno) + std::string (")");
            reply->set_message (msg);
            return grpc::Status (grpc::StatusCode::INVALID_ARGUMENT, "");
        }
        if (imagedb_insert_image (db, path.c_str ()) < 0) {
            std::string msg = std::string ("ImageDB: imagedb_insert_image (")
                                  + strerror (errno) + std::string (")");
            reply->set_message (msg);
            return grpc::Status (grpc::StatusCode::INVALID_ARGUMENT, "");
        }

        sqlite3_close (db);
        reply->set_message("Success");

        return Status::OK;
    }
    std::string dbpath;
};


static void run_server (server_ctx_t *ctx)
{
    std::string server_address (ctx->target);
    ImageDbServiceImpl service;

    if (access (ctx->dbpath.c_str (), F_OK) != 0) {
        std::cerr << "ImageDB: "
                  << ctx->dbpath << " does not exist." << std::endl;
        exit (1);
    }

    service.dbpath = ctx->dbpath;

    grpc::EnableDefaultHealthCheckService (true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin ();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort (server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService (&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server (builder.BuildAndStart());
    std::cout << "ImageDb Server listening on " << server_address << std::endl;
  
    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv)
{ 
    server_ctx_t ctx;
    
    process_args (&ctx, argc, argv);

    run_server (&ctx);
 
    return 0;
}


/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
