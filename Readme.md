# Repo to parse Market Data from CoinBase and calculate some statistics over it

### Building
- `git submodule update --init --recursive`
- `cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug .`
- `cd build && make`