// Copyright (c) 2017-2020, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)

#ifndef INLET_TEST_UTILS_HPP
#define INLET_TEST_UTILS_HPP

#include <vector>
#include <string>

#include "gtest/gtest.h"

#include "axom/inlet/YAMLReader.hpp"

#ifdef AXOM_USE_SOL
  #include "axom/inlet/LuaReader.hpp"
#endif

namespace axom::inlet::detail
{
class LuaToYAML
{
  static std::vector<std::string> tokenize(const std::string& text);

public:
  static std::string convert(const std::string& luaString);
};

template <typename InletReader>
inline std::string fromLuaTo(const std::string& luaString)
{
  return luaString;
}

template <>
inline std::string fromLuaTo<axom::inlet::YAMLReader>(const std::string& luaString)
{
  return LuaToYAML::convert(luaString);
}

#ifdef AXOM_USE_SOL
using ReaderTypes =
  ::testing::Types<axom::inlet::LuaReader, axom::inlet::YAMLReader>;
#else
using ReaderTypes = ::testing::Types<axom::inlet::YAMLReader>;
#endif

}  // namespace axom::inlet::detail

#endif