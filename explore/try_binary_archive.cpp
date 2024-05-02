//
// Created by ADMIN on 01-May-24.
//
#include <iostream>
#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <sstream>

// Example Serializable class
class MySerializableClass {
public:
  int value1{42};
  double value2{24};

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & value1;
    ar & value2;
  }
};
using byte_ostream = std::basic_ostream<std::byte>;
using byte_istream = std::basic_istream<std::byte>;
int main() {
  std::vector<char> byteVector;

  // Example objects
  MySerializableClass obj0;
  obj0.value1 = 0;
  obj0.value2 = 0;

  MySerializableClass obj1;
  obj1.value1 = 42;
  obj1.value2 = 3.14;

  MySerializableClass obj2;
  obj2.value1 = 100;
  obj2.value2 = 2.718;

  // Create a stringstream to serialize to
  std::basic_streambuf<std::byte> buf;
  byte_ostream oss(&buf);
  boost::archive::binary_oarchive oa(oss);



  // Serialize objects into the archive
  oa << obj0;
  oa << obj1;
  oa << obj2;


//  std::vector<int> cum{1,2,3,4,5,6,7};
//  oa.save_array(cum);
  // Get the byte vector from the stringstream
  std::string serializedData = oss.str();
  byteVector.assign(serializedData.begin(), serializedData.end());

  // Example: Print bytes in the vector
  std::cout << byteVector.size() << std::endl;
  for (auto byte : byteVector) {
    std::cout << std::hex << static_cast<uint8_t>(byte);
  }
  std::cout << std::endl;

  return 0;
}
