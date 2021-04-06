set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(market "market.cpp"
"order.h" "order.cpp"
"cache.h" "cache.cpp"
"test.h" "test.cpp"
)
