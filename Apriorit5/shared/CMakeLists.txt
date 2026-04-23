set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/protos")
set(GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

file(MAKE_DIRECTORY ${GEN_DIR})
find_program(gRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

add_custom_command(
	OUTPUT "${GEN_DIR}/file_transfer.pb.cc"
           "${GEN_DIR}/file_transfer.pb.h"
           "${GEN_DIR}/file_transfer.grpc.pb.cc"
           "${GEN_DIR}/file_transfer.grpc.pb.h"
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS --grpc_out="${GEN_DIR}"
         --cpp_out="${GEN_DIR}"
         --plugin=protoc-gen-grpc="${gRPC_CPP_PLUGIN_EXECUTABLE}"
         -I "${PROTO_DIR}"
         "${PROTO_DIR}/file_transfer.proto"
    DEPENDS "${PROTO_DIR}/file_transfer.proto"
)

add_library(GrpcProtoLib STATIC
	"${GEN_DIR}/file_transfer.pb.cc"
	"${GEN_DIR}/file_transfer.grpc.pb.cc"
)

target_include_directories(GrpcProtoLib PUBLIC
	"${GEN_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(GrpcProtoLib PUBLIC gRPC::grpc++ protobuf::libprotobuf)
