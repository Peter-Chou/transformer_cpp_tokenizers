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

put the following code in you CMakeLists.txt

``` cmake
set(tokenizers_DIR /opt/tokenizers/lib/cmake)
find_package(tokenizers REQUIRED) # target: tokenizers::tokenizers

```
