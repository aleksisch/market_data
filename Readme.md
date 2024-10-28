# Repo to parse Market Data from CoinBase and calculate some statistics over it

### Building
- `git submodule update --init --recursive`
- `cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/clang++ .`
- `cd build && make`

We forced to use `clang`, because `google FuzzTest` is not compatible with `gcc` yet.

### Run

```
./coinbase_parser -f coinbase.csv --products BTC-USD ETH-USD
```
Will launch session which will subscribe to `BTC-USD` and `ETH-USD` and write to `coinbase.csv` 