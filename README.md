# tokenizers

## prerequisites

install icu

``` bash
sudo apt-get install libicu-dev
```

## usage

### option1: include tokenizers from source

``` cmake
add_subdirectory(tokenizers) # target: tokenizers::tokenizers
```

### option2: build and use library by find_package

#### build and install

``` bash
cmake -B build
# cmake -B build -DBUILD_TESTING=ON -DTOKENIZERS_BUILD_TESTING=ON # enable testing
cmake --build build -j $(nproc)
install_path=/opt/tokenizers
sudo cmake --install build --prefix $install_path
```

#### find the library

``` cmake
find_package(tokenizers REQUIRED) # target: tokenizers::tokenizers
```

## usage

please go to wiki for detailed guide.
