/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2017-2018, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-741217
 *
 * All rights reserved.
 *
 * This file is part of Axom.
 *
 * For details about use and distribution, please read axom/LICENSE.
 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * \file DynamicSet.hpp
 *
 * \brief Contains a DynamicSet class, whose size can change dynamically
 * at runtime
 */

#ifndef SLAM_DYNAMIC_SET_H_
#define SLAM_DYNAMIC_SET_H_

#include "slam/OrderedSet.hpp"

namespace axom
{
namespace slam
{


/**
 * \class DynamicSet
 * \brief A Set class that supports dynamically adding and removing set items
 *
 * \detail An entry in the set is valid if it is not equal to INVALID_ENTRY.
 * Set entries should be positive integers or INVALID_ENTRY.
 *
 * An example to traverse the elements
 * \code
 * DynamicSet<> some_set;
 * const int N = some_set.size()
 * for(IndexType i=0; i< N; ++i)
 * {
 *   if( some_set.isValidEntry(i) )
 *   {
 *     ElementType el = some_set[i];
 *
 *     ... // do something with el
 *
 *   } // END if the entry is valid
 * } //END for all set entries
 *
 * \endcode
 */

template<
  typename SizePolicy    = policies::DynamicRuntimeSize<Set::PositionType>,
  typename OffsetPolicy  = policies::ZeroOffset<Set::PositionType>,
  typename StridePolicy  = policies::StrideOne<Set::PositionType> >
class DynamicSet
  : public Set,
           SizePolicy,
           OffsetPolicy,
           StridePolicy
{

public:
  typedef Set::PositionType PositionType;
  typedef Set::IndexType IndexType;
  typedef Set::ElementType ElementType;

  typedef std::vector<ElementType>  SetVectorType;

  typedef SizePolicy SizePolicyType;

  enum
  {
    INVALID_ENTRY = -1
  };

  struct SetBuilder;

public:
  DynamicSet(PositionType size = SizePolicyType::DEFAULT_VALUE) :
    SizePolicy(size)
  {
    fill_array_default( size );
  };

  DynamicSet(const SetBuilder & builder) :
    SizePolicy(builder.m_size),
    OffsetPolicy(builder.m_offset),
    StridePolicy(builder.m_stride)
  {
    fill_array_default( builder.m_size.size() );
  }

  //~DynamicSet();


public:
  /**
   * \class SetBuilder
   * \brief Helper class for constructing a dynamic set.
   */
  struct SetBuilder
  {
    friend class DynamicSet;

    SetBuilder& size(PositionType sz)
    {
      m_size   = SizePolicy(sz);
      return *this;
    }

    SetBuilder& offset(PositionType off)
    {
      m_offset = OffsetPolicy(off);
      return *this;
    }

    SetBuilder& stride(PositionType str)
    {
      m_stride = StridePolicy(str);
      return *this;
    }

private:
    SizePolicy m_size;
    OffsetPolicy m_offset;
    StridePolicy m_stride;
  };


  ElementType at(PositionType pos) const
  {
    return operator[](pos);
  };

  ElementType operator[](IndexType pos) const
  {
    verifyPosition(pos);
    return m_data[pos];
  };

  ElementType& operator[](IndexType pos)
  {
    verifyPosition(pos);
    return m_data[pos];
  };

  bool isValidEntry(IndexType i) const
  {
    return i >= 0
           && i < static_cast<IndexType>(m_data.size())
           && m_data[i] != INVALID_ENTRY;
  };

  /**
   * \brief return the number of valid entries in the set.
   *
   * \detail This is an O(n) operation, because the class makes no assumption
   * that data was not changed by the user
   */
  PositionType numberOfValidEntries() const
  {
    PositionType nvalid = 0;

    const int sz = static_cast<int>(m_data.size());
    for( int i=0 ; i< sz ; ++i)
    {
      nvalid += (m_data[i] != INVALID_ENTRY) ? 1 : 0;
    }
    return nvalid;
  }

  PositionType size() const
  {
    return m_data.size();
  };

  bool empty() const { return SizePolicy::empty(); };

  bool isSubset() const { return false; };

  bool isValid(bool verboseOutput = false)  const
  {
    bool bValid =  SizePolicy::isValid(verboseOutput)
                  && OffsetPolicy::isValid(verboseOutput)
                  && StridePolicy::isValid(verboseOutput);

    return bValid;
  };

  SetVectorType & data(){ return m_data; }
  const SetVectorType & data() const { return m_data; }

public:
  /* Modifying functions */

  /**
   * \brief insert an entry at the end of the set with value = ( size()-1 )
   */
  IndexType insert(){
    return insert(m_data.size());
  }

  /**
   * \brief insert an entry at the end of the set with the given value.
   * \param val the value of the inserted entry
   */
  IndexType insert(ElementType val)
  {
    m_data.push_back(val);
    return m_data.size()-1;
  };

  /**
   * \brief Mark the corresponding entry as invalid
   */
  void remove(IndexType idx)
  {
    verifyPosition(idx);

    if(m_data[idx] != INVALID_ENTRY)
    {
      m_data[idx] = INVALID_ENTRY;
    }
  };

  /**
   * \brief Given a value, find the index of the first entry containing it
   */
  IndexType findIndex(ElementType e)
  {
    for(unsigned int i=0 ; i<m_data.size() ; ++i)
    {
      if( m_data[i] == e)
        return i;
    }
    return INVALID_ENTRY;
  };


private:
  void verifyPosition(PositionType pos) const
  {
    SLIC_ASSERT_MSG(
      (pos >= 0) && (pos < static_cast<PositionType>(m_data.size() ) ),
      "SLAM::DynamicSet -- requested out-of-range element at position "
      << pos << ", but set only has " << m_data.size() << " elements." );
  };

  //Fill up m_data with the size where every entry value is its index.
  void fill_array_default(PositionType size)
  {
    if(size<0) return;

    m_data.resize(size);
    for(int i=0 ; i<size ; ++i)
    {
      m_data[i] = i;
    }
  }

private:
  SetVectorType m_data;

};


} // end namespace slam
} // end namespace axom

#endif
