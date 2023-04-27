#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <iostream>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( string data )
{
  if(is_closed()||data.empty()||error_) {
    return;
  } 
  int len = min (available_capacity(), data.length());
  pushed += len;
  queue_.append(std::move(data.substr(0,len)));
  // std::cout << "Push: " << queue_ << std::endl;
}

void Writer::close()
{
  closed_ = true;
  // Your code here.
}

void Writer::set_error()
{
  error_ = true;
  // Your code here.
}

bool Writer::is_closed() const
{
  return closed_;
  // Your code here.
}

uint64_t Writer::available_capacity() const
{
  return capacity_-pushed+popped;
  // Your code here.
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return pushed;
}

string_view Reader::peek() const
{
  // Your code here.
    return string_view(queue_.begin(), queue_.end());
}

bool Reader::is_finished() const
{
  // Your code here.
  return queue_.empty() && closed_;
}

bool Reader::has_error() const
{
  // Your code here.
  return error_;
}

void Reader::pop( uint64_t len )
{
  int size = min(len, queue_.length());
  popped += size;
  string ans =queue_.substr(size, queue_.length() - size);
  queue_ = std::move(ans);
  // Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return pushed - popped;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return popped;
}