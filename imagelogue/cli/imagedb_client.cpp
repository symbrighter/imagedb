#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "imagedb.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using imagedb::ImageDb;
using imagedb::ImageDbInsertReply;
using imagedb::ImageDbInsertRequest;


class ImageDbClient {
public:
    ImageDbClient (std::shared_ptr<Channel> channel)
        : stub_ (ImageDb::NewStub (channel)) {}
   
    std::string insert (const std::string& path)
    {
        ImageDbInsertRequest request;
        request.set_path (path);
   
        ImageDbInsertReply reply;
   
        ClientContext context;
   
        Status status = stub_->insert (&context, request, &reply);
   
        if (status.ok ()) {
            return reply.message ();
        } else {
            std::cout << status.error_code () << ": " << status.error_message ()
                      << std::endl;
            return reply.message ();
        }
    }
   
private:
 std::unique_ptr<ImageDb::Stub> stub_;
};

int main(int argc, char **argv) {
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint specified by
    // the argument "--target=" which is the only expected argument.
    // We indicate that the channel isn't authenticated (use of
    // InsecureChannelCredentials()).
    std::string target_str;
    std::string arg_str("--target");
    if (argc > 1) {
        std::string arg_val = argv[1];
        size_t start_pos = arg_val.find(arg_str);
        if (start_pos != std::string::npos) {
            start_pos += arg_str.size();
            if (arg_val[start_pos] == '=') {
                target_str = arg_val.substr(start_pos + 1);
            } else {
                std::cout << "The only correct argument syntax is --target="
                          << std::endl;
                return 0;
            }
        } else {
            std::cout << "The only acceptable argument is --target=" << std::endl;
            return 0;
        }
    } else {
        target_str = "localhost:50051";
    }
    ImageDbClient imagedb_client (grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    std::string path ("data/20220527/wolf-howling.jpg");
    std::string reply = imagedb_client.insert (path);
    std::cout << "ImageDb received: " << reply << std::endl;
    
    return 0;
}


