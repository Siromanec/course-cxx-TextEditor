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

class byte_ostream {
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
  byte_ostream &operator<<(const T &item) {
    container.resize(container.size() + sizeof(T));
    std::memmove(container.data() + container.size() - sizeof(T), &item, sizeof(T));
    return *this;
  }

  template<class T>
  requires std::is_trivial_v<typename std::span<T>::value_type>
  byte_ostream &operator<<(const std::span<T> item) {
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

  byte_ostream &operator<<(const byte_ostream &s) {
    container.insert(container.end(), s.container.begin(), s.container.end());
    return *this;
  }

};

/**
 * performs no bounds checks
 * */
class byte_istream {
  typedef std::vector<std::byte> container_type;
  container_type container;
  std::size_t offset{};
  std::size_t size_m{};


  void a() {
    int b;
    std::vector<int> c(5);
    *this >> b;
    *this >> c;
  }

public:
  void set_size(std::size_t size_) {
    size_m = size_;
  }
  void reset() {
    offset = 0;
    size_m = 0;
  }

  template<class T>
  requires std::is_trivial_v<T>
  byte_istream &operator>>(T &item) {
    std::memmove(&item, container.data() + offset, sizeof(T));
    offset += sizeof(T);
    return *this;
  }
  /*
   * expects preallocated storage
   * */
  template<class T>
  requires requires(T item) {
    item.data();
    requires std::is_trivial_v<typename std::iterator_traits<decltype(item.begin())>::value_type>;
    requires std::random_access_iterator<decltype(item.begin())>;
  }
           && (!std::is_trivial_v<T>)


  byte_istream &operator>>(T &item) {
    typedef typename std::iterator_traits<decltype(item.begin())>::value_type contained_t;
    size_t size_ = sizeof(contained_t) * (item.end() - item.begin());
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

  [[nodiscard]] container_type &getContainer() {
    return container;
  }

  [[nodiscard]] std::size_t size() const {
    return container.size();
  }

};

class serializable {
public:
  virtual byte_ostream &serialize(byte_ostream &) const = 0;

  friend byte_ostream &operator<<(byte_ostream &o, const serializable &s) {
    return s.serialize(o);
  }

  virtual byte_istream &deserialize(byte_istream &) = 0;

  friend byte_istream &operator>>(byte_istream &o, serializable &s) {
    return s.deserialize(o);
  }
};


#endif //MOTION_SERIALIZATION_HPP
