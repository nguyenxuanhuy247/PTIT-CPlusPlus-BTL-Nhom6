cmake_minimum_required(VERSION 3.10)
project(RewardWalletApp)

# Dùng C++17 để dùng std::filesystem
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include header folders
include_directories(
    include
    entities
    lib/sha
    lib 
)

# Tìm toàn bộ file .cpp trong src và các thư mục con
file(GLOB_RECURSE SOURCES src/*.cpp)

# Tạo executable
add_executable(reward_wallet ${SOURCES})
