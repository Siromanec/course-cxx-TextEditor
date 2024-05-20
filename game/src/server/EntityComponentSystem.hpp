//
// Created by ADMIN on 19-May-24.
//

#ifndef MOTION_ENTITYCOMPONENTSYSTEM_HPP
#define MOTION_ENTITYCOMPONENTSYSTEM_HPP

#include <optional>
#include <numeric>
#include <vector>
#include <tuple>
#include <cassert>
#include "serialization.hpp"

//https://programmingpraxis.com/2012/03/09/sparse-sets/
template <typename T>
class sparse_set: public serializable {
  typedef std::size_t index_t;
  typedef T value_t;
  typedef std::vector<value_t> data_container_t;
  typedef std::vector<index_t> dense_container_t;
  typedef std::vector<index_t> sparse_container_t;
  typedef data_container_t::iterator iterator;
  typedef data_container_t::const_iterator const_iterator;
  data_container_t data;
  dense_container_t dense;
  sparse_container_t sparse;
  const std::size_t universe_size;
public:
  explicit sparse_set(std::size_t universe_size) :
      universe_size(universe_size),
      sparse(universe_size, std::numeric_limits<std::size_t>::max()) {}

  constexpr
  void insert(index_t i, value_t && value) {
    if (sparse[i] < dense.size()) {
      // component already exists
      data[sparse[i]] = std::forward<value_t>(value);
      dense[sparse[i]] = i;
      return;
    }
    dense.emplace_back(i);
    data.emplace_back(std::forward<T>(value));
    sparse[i] = dense.size() - 1;
  }

  constexpr
  void erase(index_t i) {
    auto last = dense.back();
    dense[sparse[i]] = last;
    sparse[last] = sparse[i];
    pop_back();
  }
  void pop_back() {
    auto last = dense.back();
    dense.pop_back();
    data.pop_back();
    sparse[last] = std::numeric_limits<std::size_t>::max();
  }
  constexpr
  void swap(index_t i, index_t j) {
    // swaps entities
    std::swap(data[sparse[i]], data[sparse[j]]);
    std::swap(dense[sparse[i]], dense[sparse[j]]);
    std::swap(sparse[i], sparse[j]);
  }
  constexpr
  void swap_dense(index_t i, index_t j) {
    // swaps entities
//    std::swap(data[sparse[i]], data[sparse[j]]);
//    std::swap(dense[sparse[i]], dense[sparse[j]]);
//    std::swap(sparse[i], sparse[j]);

    std::swap(dense[i], dense[j]);
    std::swap(data[i], data[j]);
    std::swap(sparse[dense[i]], sparse[dense[j]]);

  }

  [[nodiscard]] const dense_container_t &get_dense() const {
    return dense;
  }
  [[nodiscard]] const sparse_container_t &get_sparse() const {
    return sparse;
  }
  [[nodiscard]] const data_container_t &get_data() const {
    return data;
  }


  // not implementing contains method because it would lead to bad design (seductive if statements)

  [[nodiscard]] constexpr
  T &operator[](index_t i) {
    return data[sparse[i]];
  }
  constexpr
  const T &operator[](index_t i) const {
    return data[sparse[i]];
  }

  constexpr
  iterator begin() {
    return data.begin();
  }
  constexpr
  iterator end() {
    return data.end();
  }
  [[nodiscard]] constexpr
  const_iterator begin() const {
    return data.begin();
  }
  [[nodiscard]] constexpr
  const_iterator end() const {
    return data.end();
  }
  void swap_dense_if_contains(index_t i, index_t j) {
    if (j < (dense.size())) {
      assert(i < dense.size());
      swap_dense(i, j);
    }
  }
  byte_ostream &serialize(byte_ostream &o) const final {
    // needs to send only the data and dense
    // size is already written by operator <<
    o << std::span(dense);
    o << std::span(data);
    return o;
  }
  byte_istream &deserialize(byte_istream &o) final {
    // todo rewrite operator >> for every component

    std::size_t dense_size_in_bytes;
    std::size_t data_size_in_bytes;
    o >> dense_size_in_bytes;
    std::size_t dense_size = dense_size_in_bytes / sizeof(index_t);
    std::vector<index_t> dense_(dense_size);
    o >> std::span(dense_);
    o >> data_size_in_bytes;
    std::size_t data_size = data_size_in_bytes / sizeof(value_t);
    std::vector<value_t> data_(data_size);
    o >> std::span(data_);
    dense = std::move(dense_);
    data = std::move(data_);
    for (std::size_t i = 0; i < dense.size(); ++i) {
      sparse[dense[i]] = i;
    }
    return o;
  }

};

/* component group. each component must create strict one directional set hierarchy (aka position is superset of direction and direction is superset of speed)
 * otherwise the behavior is undefined
 * referencing a component that does not belong to the universe is undefined behavior
 * */
template <typename ...Types>
class ComponentGroup: public serializable {
  const std::size_t universe_size;
  std::tuple<sparse_set<Types>...> sets;

public:
  explicit ComponentGroup(std::size_t universe_size) :
      universe_size(universe_size),
      sets(std::make_tuple(sparse_set<Types>{universe_size}...)) {}
  template <typename Component>
  [[nodiscard]] constexpr
  sparse_set<Component> &get() {
    return std::get<sparse_set<Component>>(sets);
  }
  template <typename Component>
  [[nodiscard]] constexpr
  const sparse_set<Component> &get() const {
    return std::get<sparse_set<Component>>(sets);
  }
  template <typename Component>
  void erase(std::size_t i) {
    // deletes a component and reorders the dense array to keep all items aligned

    auto & set_to_be_erased = std::get<sparse_set<Component>>(sets);
    auto size = set_to_be_erased.get_dense().size();
    auto j = size - 1;

    std::apply([i, j](auto &... set) {
      (set.swap_dense_if_contains(set.get_sparse()[i], j), ...);
    }, sets);

    set_to_be_erased.pop_back();
  }
  template <typename Component>
  void insert(std::size_t i, Component && component) {
    // insert a component anywhere and keep the dense array aligned
    auto & set_to_be_inserted = std::get<sparse_set<Component>>(sets);
    auto size = set_to_be_inserted.get_dense().size();
    auto j = set_to_be_inserted.get_sparse()[i];
    if (j >= size) {
      j = size;
    }

    std::apply([i, j](auto &... set) {
      (set.swap_dense_if_contains(set.get_sparse()[i], j), ...);
    }, sets);

    set_to_be_inserted.insert(i, std::forward<Component>(component));
  }
  byte_ostream &serialize(byte_ostream &o) const final {
    std::apply([&o](auto &... set) {
      (set.serialize(o), ...); // you d better be in the same order every time
    }, sets);
    return o;
  }
  byte_istream &deserialize(byte_istream &o) final {
    std::apply([&o](auto &... set) {
      (set.deserialize(o), ...); // you d better be in the same order every time
    }, sets);
    return o;
  }



};
#endif //MOTION_ENTITYCOMPONENTSYSTEM_HPP
