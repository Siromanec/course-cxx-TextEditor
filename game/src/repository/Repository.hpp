//
// Created by ADMIN on 03-Mar-24.
//

#ifndef MOTION_REPOSITORY_HPP
#define MOTION_REPOSITORY_HPP

#include <vector>
template<class T>
class Repository {
  std::vector<T> data;
public:
  virtual void add(T&& datum);
  virtual const std::vector<T> & getAll() const;
};


template<class T>
void Repository<T>::add(T && datum) {
  data.emplace_back(std::move(datum));
}

template<class T>
const std::vector<T> &Repository<T>::getAll() const {
  return data;
}


#endif //MOTION_REPOSITORY_HPP
