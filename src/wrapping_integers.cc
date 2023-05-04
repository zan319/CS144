#include "wrapping_integers.hh"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
using namespace std;
Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.
  return zero_point + static_cast<uint32_t>(n);
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  uint64_t offset = raw_value_ - zero_point.raw_value_;
  uint64_t temp = (checkpoint & 0xffffffff00000000) + offset;
  uint64_t ret = temp;
  if(abs(int64_t(temp + (1ul<<32) - checkpoint)) < abs(int64_t(temp - checkpoint))) {
    ret = ret + (1ul << 32);
  }
  if(temp >= (1ul << 32) && abs(int64_t(temp - (1ul << 32) - checkpoint)) < abs(int64_t(ret - checkpoint))) {
    ret = temp - (1ul << 32);
  }
  return ret;
}
