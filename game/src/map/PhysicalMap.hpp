//
// Created by ADMIN on 01-Mar-24.
//

#ifndef MOTION_PHYSICALMAP_HPP
#define MOTION_PHYSICALMAP_HPP


#include <utility>
#include <glm/vec2.hpp>
class PhysicalMap {
  glm::vec2 bounds;
public:
  bool hasStraightLine(const glm::vec2& A, const glm::vec2& B) {
    return true;
  }
  bool hasLineOfSight(const glm::vec2& A, const glm::vec2& B) {
    return hasStraightLine(A, B);
  }
};


#endif //MOTION_PHYSICALMAP_HPP
