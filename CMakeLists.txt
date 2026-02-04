cmake_minimum_required(VERSION 3.15)
project(grpc_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Proto/chat.proto)

# Protobuf kaynakları
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})

# gRPC kaynakları
set(GRPC_SRCS ${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.cc)
set(GRPC_HDRS ${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.h)

add_custom_command(
    OUTPUT ${GRPC_SRCS} ${GRPC_HDRS}
    COMMAND protobuf::protoc
    ARGS
        --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
        --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
        -I ${CMAKE_CURRENT_SOURCE_DIR}/Proto
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
)

# proto_lib kütüphanesi
add_library(proto_lib
    ${PROTO_SRCS} ${PROTO_HDRS}
    ${GRPC_SRCS}  ${GRPC_HDRS}
)
target_include_directories(proto_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(proto_lib PUBLIC protobuf::libprotobuf gRPC::grpc++)

# Proje içeriği
add_library(core_lib
    Core/ChatClient.cpp
    Core/ChatServiceImpl.cpp
)
target_include_directories(core_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(core_lib PUBLIC proto_lib)

add_executable(server Server/main.cpp)
target_link_libraries(server PRIVATE core_lib)

add_executable(client Client/main.cpp)
target_link_libraries(client PRIVATE core_lib)
