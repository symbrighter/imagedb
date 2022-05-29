AM_CFLAGS = -I$(top_srcdir) $(PROTOBUF_CFLAGS) $(GRPCPP_CFLAGS)

MOSTLYCLEANFILES = imagedb.pb.cc imagedb.pb.h imagedb.grpc.pb.cc imagedb.grpc.pb.h
BUILT_SOURCES = imagedb.pb.h

vpath %.proto ../proto

dist_noinst_DATA = imagedb.proto
noinst_PROGRAMS = imagedb-server imagedb-client

imagedb_server_SOURCES = \
	$(builddir)/imagedb.pb.cc \
	$(builddir)/imagedb.grpc.pb.cc \
	$(srcdir)/imagedb_server.cpp \
	$(builddir)/imagedb.pb.h \
	$(builddir)/imagedb.grpc.pb.h

imagedb_client_SOURCES = \
	$(builddir)/imagedb.pb.cc \
	$(builddir)/imagedb.grpc.pb.cc \
	$(srcdir)/imagedb_client.cpp \
	$(builddir)/imagedb.pb.h \
	$(builddir)/imagedb.grpc.pb.h

imagedb_server_CXXFLAGS = \
	-I$(srcdir) \
	-I$(top_srcdir)/imagelogue/db \
	$(AM_CFLAGS)

imagedb_server_LDFLAGS = \
	-pthread

imagedb_server_LDADD = \
	$(SQLITE3_LIBS) \
	$(PROTOBUF_LIBS) \
	$(GRPCPP_LIBS) \
	-lgrpc++_reflection \
	-ldl \
	../libimagedb.la

imagedb_client_CXXFLAGS = \
	-I$(srcdir) \
	-I$(top_srcdir)/imagelogue/db \
	$(AM_CFLAGS)

imagedb_client_LDFLAGS = \
	-pthread

imagedb_client_LDADD = \
	$(PROTOBUF_LIBS) \
	$(GRPCPP_LIBS) \
	-lgrpc++_reflection \
	-ldl

%.grpc.pb.cc %.grpc.pb.h: %.proto
	echo "Depedency met"
	$(PROTOC) --proto_path=../proto --grpc_out=$(builddir) \
		  --plugin=protoc-gen-grpc=/home/dahn/.local/bin/grpc_cpp_plugin $^

%.pb.cc %.pb.h: %.proto
	$(PROTOC) --proto_path=../proto --cpp_out=$(builddir) $^
