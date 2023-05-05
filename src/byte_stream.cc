#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <iostream>
#include <sstream>
#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( string data )
{
  if(is_closed()||data.empty()||error_) {
    return;
  } 
  uint64_t len = min (available_capacity(), data.length());
  pushed += len;
  // for(uint64_t i = 0; i < len; i++) {
  //   queue_.push_back(data.at(i));
  // }
  queue_.append(data.substr(0,len));
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
    // return{&queue_.front(),1};
    return {queue_.begin(), queue_.end()};
  // Your code here.
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
  uint64_t size = min(len, queue_.size());
  popped += size;
  // for(uint64_t i = 0; i < size; i++){
  //   if(!queue_.empty()){
  //     queue_.pop_front();
  //   }
  // }
  queue_.erase(0,len);
  // string ans =queue_.substr(size, queue_.length() - size);
  // queue_ = std::move(ans);
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