//
// Created by ADMIN on 01-Mar-24.
//

#ifndef MOTION_GAMEMAP_HPP
#define MOTION_GAMEMAP_HPP

#include <glm/vec4.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "PhysicalMap.hpp"
#include "Path.hpp"
#include "Graph.hpp"

class GameMap {

  const PhysicalMap&  physicalMap;
//  boost::adjacency_list<glm::vec2> graph;
//  std::vector<>

public:
  explicit GameMap(PhysicalMap&  physicalMap): physicalMap{physicalMap} {
//    boost::add_vertex(glm::vec2{}, graph);
  }

  Path getPath(const glm::vec2& A, const glm::vec2& B) {
    return Path{{B}};
  }



};


#endif //MOTION_GAMEMAP_HPP
