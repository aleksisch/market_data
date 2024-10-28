//
// Created by aleksey on 28.10.24.
//
#pragma once

class IExchange {
 public:
  virtual void subscribe_ticker() = 0;
};