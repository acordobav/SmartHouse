# SM-Server

Este proyecto utiliza CMake para realizar la compilación del software.

## Instalacion Curl
$ sudo apt update
$ sudo apt-get install libcurl4-openssl-dev

## Instalacion Pistache

### Ubuntu PPA (Unstable)

```console
$ sudo add-apt-repository ppa:pistache+team/unstable
$ sudo apt update
$ sudo apt install libpistache-dev
```

### Building from source

```console
$ git clone https://github.com/oktal/pistache.git
$ git submodule update --init
$ cd pistache
$ mkdir -p {build,prefix}
$ cd build
$ cmake -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPISTACHE_BUILD_EXAMPLES=true \
    -DPISTACHE_BUILD_TESTS=true \
    -DPISTACHE_BUILD_DOCS=false \
    -DPISTACHE_USE_SSL=true \
    -DCMAKE_INSTALL_PREFIX=$PWD/../prefix \
    ../
$ make -j
$ make install
```

## Instalacion de RapidJson

```console
$ sudo apt-get update
$ sudo apt-get install rapidjson-dev
```

## Instalacion de Boost

```console
$ sudo apt-get update
$ sudo apt-get install libboost-all-dev
```