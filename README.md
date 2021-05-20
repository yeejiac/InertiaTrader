# InertiaTrader


[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

This project is mainly produce to simulate a stock/future trading system
  

## Features

- Simulate the financial product trading
- One by one matching method
- Immediate settlement contract
- Immediate account detail provide 
- Databases replication


## Tech

Inertia Trader uses several manner to make sure the trading stability:

- C++
- MariaDB
- node.js - evented I/O for the backend
- Express - fast node.js network app framework [@tjholowaychuk]

## Installation

Install the dependencies and devDependencies and start the server.

```sh
cd InertiaTrader
make main
./trader.out devel
```

## Plugins

Instructions on how to use them in your own application are linked below.

| Plugin | README |
| ------ | ------ |
| GitHub | [InertiaTrader/github/README.md][PlGh] |


## Docker

InertiaTrader is very easy to install and deploy in a Docker container.

Dockerised InertiaTrader(port 1203)

```sh
docker build -t inertiatrader .
docker run -it inertiatrader ./trader.out devel -- --coverage
```

Dockerised Database(port 3306)

```sh
docker build -f dockerfile.db -t mydb .
docker run -d -p 3306:3306 --name mydb-container mydb
```

Dockerised trading environment deploy

```sh
docker-compose up -d
```
