//
// Created by ADMIN on 01-May-24.
//
// bruv decided "helper" is too simple skeleton_emoji, skeleton_emoji, skeleton_emoji
#ifndef MOTION_SERIALIZATION_HPP
#define MOTION_SERIALIZATION_HPP

#include <cstddef>
#include <vector>
#include <span>
#include <memory>
#include <cstring>

class byte_stream {
  typedef std::vector<std::byte> container_type;
  container_type container;

  void a() {
    std::vector<int> v(5);
    std::span s(v);
    *this << s;
  }

public:
  template<class T>
  requires std::is_trivial_v<T>
  byte_stream &operator<<(const T &item) {
    container.resize(container.size() + sizeof(T));
    std::memmove(container.data() + container.size() - sizeof(T), &item, sizeof(T));
    return *this;
  }

  template<class T>
  requires std::is_trivial_v<typename std::span<T>::value_type>
  byte_stream &operator<<(const std::span<T> item) {
    typedef typename std::iterator_traits<decltype(item.begin())>::value_type contained_t;
    size_t size_ = item.size_bytes();
    *this << size_;
    container.resize(container.size() + size_);
    std::memmove(container.data() + container.size() - size_, item.data(), size_);
    return *this;
  }

  [[nodiscard]] const container_type &getContainer() const {
    return container;
  }

  [[nodiscard]] std::size_t size() const {
    return container.size();
  }

  byte_stream &operator<<(const byte_stream &s) {
    container.insert(container.end(), s.container.begin(), s.container.end());
    return *this;
  }

};
class byte_istream {
  typedef std::vector<std::byte> container_type;
  container_type container;
  std::size_t offset{};

  void a() {
    int b;
    std::vector<int> c(5);
    *this >> b;
    *this >> c;
  }

public:
  void reset() {
    offset = 0;
  }
  template<class T>
  requires std::is_trivial_v<T>
  byte_istream &operator>>(T &item) {
    std::memmove(&item, container.data() + offset, sizeof(T));
    offset += sizeof(T);
    return *this;
  }
  template<class T>
  requires requires(T item) {
    item.data();
    requires std::is_trivial_v<typename std::iterator_traits<decltype(item.begin())>::value_type>;
    requires std::random_access_iterator<decltype(item.begin())>;
  }
           && (!std::is_trivial_v<T>)


  byte_istream &operator>>(T &item) {
    // expects preallocated storage
    typedef typename std::iterator_traits<decltype(item.begin())>::value_type contained_t;
    size_t size_ = sizeof(contained_t) * (item.end() - item.begin());
//    container.resize(container.size() + size_);
    std::memmove(item.data(), container.data() + offset, size_);
    offset += size_;
    return *this;
  }

  template<class T>
  requires std::is_trivial_v<typename std::span<T>::value_type>
  byte_istream &operator>>(std::span<T> item) {
    size_t size_ = item.size_bytes();
    std::memmove(item.data(), container.data() + offset, size_);
    offset += size_;
    return *this;
  }
//  byte_stream &operator<<(const byte_stream &s) {
//    container.insert(container.end(), s.container.begin(), s.container.end());
//    return *this;
//  }
  [[nodiscard]] container_type &getContainer() {
    return container;
  }

  [[nodiscard]] std::size_t size() const {
    return container.size();
  }



};

class serializable {
public:
  virtual byte_stream & serialize(byte_stream &) const = 0;
  friend byte_stream &operator<<(byte_stream &o, const serializable &s) {
    return s.serialize(o);
  }

  virtual byte_istream & deserialize(byte_istream &) = 0;
  friend byte_istream &operator>>(byte_istream &o, serializable &s) {
    return s.deserialize(o);
  }
};

#endif //MOTION_SERIALIZATION_HPP
