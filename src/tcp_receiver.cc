#include "tcp_receiver.hh"
#include "reassembler.hh"
#include "tcp_receiver_message.hh"
#include "wrapping_integers.hh"
#include <cstdint>
#include <optional>
#include <iostream>

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message, Reassembler& reassembler, Writer& inbound_stream )
{
  // Your code here.
  // (void)message;
  // (void)reassembler;
  // (void)inbound_stream;

  if(message.SYN) {
    SYN_set = true;
    zero_point = message.seqno;
  }
  if(SYN_set && message.FIN) {
    FIN_set = true;
  }

  uint64_t first_index = (message.SYN == false) ? 
    Wrap32(message.seqno).unwrap(zero_point, inbound_stream.bytes_pushed() + SYN_set + FIN_set): 1;
  
  reassembler.insert(first_index - 1, message.payload, FIN_set, inbound_stream);
  std::cout << "first_index : " << first_index << "data : " << message.payload.release() << std::endl;
  reassembler_in = reassembler.bytes_pending();
  // ab_sqno += message.sequence_length();

}

TCPReceiverMessage TCPReceiver::send( const Writer& inbound_stream ) const
{
    // Your code here.
  uint16_t window_size = inbound_stream.available_capacity() > 0xffff ?
      0xffff : inbound_stream.available_capacity();
  if(!SYN_set){
    return {optional<Wrap32>{}, window_size};
  }
  bool def_end = false;
  if(FIN_set && reassembler_in == 0) {
    def_end = true;
  }
  // cout << "bytes_pushed  " << inbound_stream.bytes_pushed() << "window_size : " << window_size << endl;
  return TCPReceiverMessage{
    optional<Wrap32>{Wrap32(0).wrap(inbound_stream.bytes_pushed() + SYN_set + def_end ,zero_point)}, window_size };
}