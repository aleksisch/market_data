# Repo to parse Market Data from CoinBase and calculate some statistics over it

### Building
- `git submodule update --init --recursive`
- `cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/clang++ .`
- `cd build && make`

We forced to use `clang`, because `google FuzzTest` is not compatible with `gcc` yet.
