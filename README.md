# tokenizers

## prerequisites

install icu

``` bash
sudo apt-get install libicu-dev
```

## usage

### include tokenizers from source

``` cmake
add_subdirectory(tokenizers) # target: tokenizers::tokenizers
```

### find_package

#### build

``` bash
cmake -B build
cmake --build build -j $(nproc)
```

#### install

``` bash
install_path=/opt/tokenizers
sudo cmake --install build --prefix $install_path
```

#### include tokenizers from built library

``` cmake
set(tokenizers_DIR /opt/tokenizers/lib/cmake)
find_package(tokenizers REQUIRED) # target: tokenizers::tokenizers
```

## enable debug and testing

``` bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DTOKENIZERS_BUILD_TESTING=ON
cmake --build build -j $(nproc)
ctest --test-dir build
```
