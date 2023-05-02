#include "reassembler.hh"
#include <cstdint>
#include <ratio>
#include <sys/types.h>
#include <iostream>
#include <type_traits>
using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  // cout << output.available_capacity() << endl;
  // cout << "From : "<<first_index <<"   "<<"To : "<<first_index + data.length()<< "     "<< index<< "  " <<data<< endl;
  if(is_last_substring){
    is_last = true;
  }
  // cout <<"Input  "<< first_index << "  " << first_index + data.length() - 1 <<endl;
  // cout <<"In"<< index <<"  "<<first_index + data.length() - 1<< endl;

  if(first_index >= index + output.available_capacity() || data.empty() || first_index + data.length() - 1 < index) {
    if(to_close()) {
      output.close();
    }
    return;
  }
  // cout <<"pass" <<endl;
  // auto i = mapping.find(first_index);
  // if(i == mapping.end()){
    // add_count(data, first_index);
    // in+=data.length();
  uint64_t cap = output.available_capacity();
  if(first_index < index && first_index + data.size() - 1 <= index - 1 + cap) {
    //data: from small than index to small than available_capacity
    mapping[index] = data.substr(index-first_index);
    add_count(mapping[index].length(), index);
  }else if(first_index >= index&&first_index + data.length() - 1 <= index - 1 + cap){
    //data: from larger than index to smaller than available_capacity
    mapping[first_index] = data;
    add_count(mapping[first_index].length(), first_index);
  }else if(first_index > index&&first_index + data.length() - 1 > index - 1 + cap){
    //data: from larger than index to available_capacity
    mapping[first_index] = data.substr(0, index + cap -first_index);
    add_count(cap, first_index);
  }else{
    //data : from smaller than index to larger than available_capacity
    mapping[index] = data.substr(index-first_index,cap);
    add_count(cap, index);
  }
  // cout << "size : "<<count.size() << endl;
  
  auto it = mapping.find(index);
  while(it != mapping.end()) {
      // cout << "Iterator : " << it->first << "  " << it->second  << "  " << index << endl;
      delete_count(it->second.length(), it->first);
      // cout << "deleting !!!"<<endl;
      // cout << "Delete : " << it->second << endl;
      // cout << "Set_size : " << count.size() <<endl;
      // cout <<"delete it ?"<<  output.available_capacity() << "  " << it->second.size() << endl;
    // if(output.available_capacity() >= it->second.size()){
      output.push(it->second);
      // cout <<"Output :    from : "<< it->first<<"    to : " <<it->second.length() - 1 + it->first << endl;
      // cout << "Now : "<<count.size() <<endl;
      index += it->second.length();
      
      mapping.erase(it->first);
      it = mapping.find(index);
  }


  // cout << "Index : " << index << endl;
  auto sec = mapping.begin();
  while(sec != mapping.end()){
    if(sec->first < index ){
      if(sec->first - 1 + sec->second.size() < index){
        delete_count(sec->second.length(), sec->first);
        mapping.erase(sec->first);
      }else{
        string part = sec->second.substr(index - sec->first);

        mapping[index] =std::move(part);
        delete_count(sec->second.length()-part.length(), sec->first);
        mapping.erase(sec->first);

        auto its = mapping.find(index);
        while(its != mapping.end()) {
           delete_count(its->second.length(), its->first);
            output.push(its->second);  
            //  cout <<"Output :    from : "<< its->first<<"    to : " <<its->second.size() - 1 + its->first << endl;
            //  cout <<"Remain : "<< bytes_pending() << endl;
            //  cout << "Now : "<<count.size() <<endl;
            index += its->second.length();
            mapping.erase(its->first);
            its = mapping.find(index);
        }
      }
      sec = mapping.begin();
    }else{
      break;
    }
  }
  if(to_close()) {
    output.close();
  }
}

uint64_t Reassembler::bytes_pending() 
{
  return count.size();
}

bool Reassembler::to_close(){
  return bytes_pending()==0 && is_last;
}

void Reassembler::add_count(uint64_t length, uint64_t first_index) {
  for(uint64_t i = 0; i < length; i++){
    count.insert(first_index + i);
  }
}
void Reassembler::delete_count(uint64_t length, uint64_t first_index){
  for(uint64_t i = 0; i< length; i++){
    if(count.count(first_index + i) == 1)
      count.erase(first_index + i);
  }
}