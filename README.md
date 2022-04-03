# tokenizers

## prerequisites

install icu

``` bash
sudo apt-get install libicu-dev
```

## build

``` bash
cmake -B build
cmake --build build -j $(nproc)
```

## install

``` bash
install_path=/opt/tokenizers
sudo cmake --install build --prefix $install_path
```

## usage

### include project

``` cmake
add_subdirectory(tokenizers) # target: tokenizers::tokenizers
```

### find_package

``` cmake
set(tokenizers_DIR /opt/tokenizers/lib/cmake)
find_package(tokenizers REQUIRED) # target: tokenizers::tokenizers
```

## enable testing

``` bash
cmake -B build -DBUILD_TESTING=ON -DTOKENIZERS_BUILD_TESTING=ON
cmake --build build -j $(nproc)
ctest --test-dir build
```
