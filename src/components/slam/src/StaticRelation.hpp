/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */


/**
 * \file StaticRelation.hpp
 *
 * \brief API for a topological relation between two sets where the
 *        relation does not change after it is initialized
 *
 */

#ifndef SLAM_STATIC_RELATION_HPP_
#define SLAM_STATIC_RELATION_HPP_

#include "common/config.hpp"
#include "common/AxomMacros.hpp"

#include "slam/Relation.hpp"
#include "slam/OrderedSet.hpp"

#include "slam/SizePolicies.hpp"
#include "slam/StridePolicies.hpp"
#include "slam/OffsetPolicies.hpp"
#include "slam/IndirectionPolicies.hpp"
#include "slam/PolicyTraits.hpp"


namespace axom {
namespace slam    {


namespace policies {

  template<
    typename ElementType = int,
    typename StridePolicy = RuntimeStrideHolder<ElementType> >
  struct ConstantCardinalityPolicy
  {
    typedef RuntimeSizeHolder<ElementType>          BeginsSizePolicy;
    typedef ZeroOffset<ElementType>                 BeginsOffsetPolicy;
    typedef StridePolicy                            BeginsStridePolicy;
    typedef NoIndirection<ElementType,ElementType>  BeginsIndirectionPolicy;

    // runtime size (fromSet.size()), striding from template parameter, no offset
    typedef OrderedSet<
          BeginsSizePolicy,
          BeginsOffsetPolicy,
          BeginsStridePolicy>                       BeginsSet;

    // The cardinality of each relational operator is determined by the StridePolicy of the relation
    typedef typename StrideToSize<
          BeginsStridePolicy,
          ElementType,
          BeginsStridePolicy::DEFAULT_VALUE>::SizeType RelationalOperatorSizeType;


    ConstantCardinalityPolicy() : m_begins() {}
    ConstantCardinalityPolicy(BeginsSet begins) : m_begins(begins) {}
    ConstantCardinalityPolicy(ElementType fromSetSize)
    {
      m_begins = BeginsSet(fromSetSize);
    }

    ConstantCardinalityPolicy(ElementType fromSetSize, typename BeginsSet::SetBuilder& builder )
    {
      // needs a size and a stride (when runtime)
      builder.size(fromSetSize);
      m_begins = builder;
    }

    const ElementType size(ElementType AXOM_NOT_USED(fromPos) ) const { return m_begins.stride(); }
    const ElementType offset(ElementType fromPos) const { return m_begins[fromPos]; }

    void              setOffsets(ElementType fromSetSize, ElementType stride)
    {
      m_begins = typename BeginsSet::SetBuilder()
          .size(fromSetSize)
          .stride(stride);
    }

    ElementType totalSize() const { return m_begins.stride() * m_begins.size(); }

    template<typename FromSetType>
    bool        isValid(const FromSetType* fromSet, bool AXOM_NOT_USED(vertboseOutput) = false) const
    {
      return m_begins.size() == fromSet->size();
    }


    BeginsSet m_begins;
  };

  template<
    typename ElementType = int,
    typename IndirectionPolicy = STLVectorIndirection<ElementType, ElementType>
  >
  struct VariableCardinalityPolicy
  {
    typedef RuntimeSizeHolder<ElementType>  BeginsSizePolicy;
    typedef ZeroOffset<ElementType>         BeginsOffsetPolicy;
    typedef StrideOne<ElementType>          BeginsStridePolicy;
    typedef IndirectionPolicy               BeginsIndirectionPolicy;

    // runtime size (fromSet.size()), striding from template parameter, no offset
    typedef OrderedSet<
          BeginsSizePolicy,
          BeginsOffsetPolicy,
          BeginsStridePolicy,
          IndirectionPolicy>                BeginsSet;


    // The cardinality of each relational operator is determined by the StridePolicy of the relation
    typedef BeginsSizePolicy                                  RelationalOperatorSizeType;

    typedef typename IndirectionPolicy::IndirectionBufferType IndirectionBufferType;

    VariableCardinalityPolicy() : m_begins() {}
    VariableCardinalityPolicy(BeginsSet begins) : m_begins(begins) {}
    VariableCardinalityPolicy(ElementType fromSetSize, typename BeginsSet::SetBuilder& builder)
    {
      builder.size(fromSetSize + 1);
      m_begins = builder;
    }

    void setOffsets(ElementType fromSetSize, IndirectionBufferType* data)
    {
      m_begins = typename BeginsSet::SetBuilder()
          .size(fromSetSize + 1)
          .data(data);
    }

    const ElementType size(ElementType fromPos) const
    {
      return offset(fromPos + 1) - offset(fromPos);
    }

    const ElementType offset(ElementType fromPos) const
    {
      return m_begins[fromPos];
    }

    ElementType totalSize() const
    {
      return m_begins.empty()
             ? ElementType()
             : offset(m_begins.size() - 1);
    }

    template<typename FromSetType>
    bool isValid(const FromSetType* fromSet, bool verboseOutput = false) const
    {
      return m_begins.size() == (fromSet->size() + 1)
             && static_cast<IndirectionPolicy>(m_begins).isValid(
        m_begins.size(), m_begins.offset(), m_begins.stride(), verboseOutput);
    }


    BeginsSet m_begins;
  };

} // end namespace policies






  template<
    typename RelationCardinalityPolicy,
    typename RelationIndicesIndirectionPolicy,
    typename TheFromSet = Set,
    typename TheToSet = Set >
  class StaticRelation : public /*Relation,*/ RelationCardinalityPolicy
  {
  public:
    typedef TheFromSet                        FromSetType;
    typedef TheToSet                          ToSetType;

    typedef Relation::SetPosition             SetPosition;

    typedef RelationCardinalityPolicy         CardinalityPolicy;
    typedef typename CardinalityPolicy::RelationalOperatorSizeType
        BeginsSizePolicy;

    typedef RelationIndicesIndirectionPolicy  IndicesIndirectionPolicy;

    typedef OrderedSet<
          BeginsSizePolicy,
          policies::RuntimeOffsetHolder<SetPosition>,
          policies::StrideOne<SetPosition>,
          IndicesIndirectionPolicy >                  RelationSet;


    typedef OrderedSet<
          policies::RuntimeSizeHolder<SetPosition>,
          policies::ZeroOffset<SetPosition>,
          policies::StrideOne<SetPosition>,
          IndicesIndirectionPolicy >                  IndicesSet;

    typedef typename IndicesIndirectionPolicy::IndirectionBufferType  IndirectionBufferType;

#ifdef AXOM_USE_BOOST
    typedef typename RelationSet::iterator                            RelationIterator;
    typedef typename RelationSet::iterator_pair                       RelationIteratorPair;

    typedef typename RelationSet::const_iterator                      RelationConstIterator;
    typedef typename RelationSet::const_iterator_pair                 RelationConstIteratorPair;
#endif // AXOM_USE_BOOST

  public:
    struct RelationBuilder;

    StaticRelation()
        : m_fromSet( EmptySetTraits<FromSetType>::emptySet() ),
          m_toSet( EmptySetTraits<ToSetType>::emptySet() )
    {}


    StaticRelation(FromSetType* fromSet, ToSetType* toSet)
        : CardinalityPolicy( EmptySetTraits<FromSetType>::isEmpty(fromSet) ? 0 : fromSet->size() ),
          m_fromSet(fromSet),
          m_toSet(toSet)
    {}

    StaticRelation(const RelationBuilder& builder)
        : CardinalityPolicy(builder.m_cardPolicy),
          m_fromSet(builder.m_fromSet),
          m_toSet(builder.m_toSet),
          m_relationIndices(builder.m_indBuilder)
    {}

    struct RelationBuilder
    {
      friend class StaticRelation;

      typedef typename StaticRelation::CardinalityPolicy::BeginsSet::SetBuilder BeginsSetBuilder;
      typedef typename StaticRelation::IndicesSet::SetBuilder                   IndicesSetBuilder;

      RelationBuilder()
          : m_fromSet( EmptySetTraits<FromSetType>::emptySet() ),
            m_toSet( EmptySetTraits<ToSetType>::emptySet() )
      {}

      RelationBuilder& fromSet(FromSetType* pFromSet)
      {
        m_fromSet = pFromSet;
        if(m_cardPolicy.totalSize() == 0 && !EmptySetTraits<FromSetType>::isEmpty(m_fromSet))
        {
          m_cardPolicy = CardinalityPolicy( m_fromSet->size() );
        }
        return *this;
      }

      RelationBuilder& toSet(ToSetType* pToSet)
      {
        m_toSet  = pToSet;
        return *this;
      }

      RelationBuilder& begins(BeginsSetBuilder& beginsBuilder)
      {
        SLIC_ASSERT_MSG( !EmptySetTraits<FromSetType>::isEmpty(m_fromSet),
            "Must set the 'fromSet' pointer before setting the begins set");

        m_cardPolicy = CardinalityPolicy( m_fromSet->size(), beginsBuilder);
        return *this;
      }
      RelationBuilder& indices(const IndicesSetBuilder& indicesBuilder)
      {
        m_indBuilder = indicesBuilder;
        return *this;
      }

    private:
      FromSetType* m_fromSet;
      ToSetType* m_toSet;
      CardinalityPolicy m_cardPolicy;
      IndicesSetBuilder m_indBuilder;
    };


  public:

    const RelationSet operator[](SetPosition fromSetInd ) const
    {
      SLIC_ASSERT( m_relationIndices.isValid(true) );

      typedef typename RelationSet::SetBuilder SetBuilder;
      return SetBuilder()
             .size( CardinalityPolicy::size( fromSetInd ) )
             .offset ( CardinalityPolicy::offset( fromSetInd ))
             .data ( m_relationIndices.data() )
      ;
    }

    RelationSet operator[](SetPosition fromSetInd )
    {
      SLIC_ASSERT( m_relationIndices.isValid(true) );

      typedef typename RelationSet::SetBuilder SetBuilder;
      return SetBuilder()
             .size( CardinalityPolicy::size( fromSetInd ) )
             .offset ( CardinalityPolicy::offset( fromSetInd ))
             .data ( m_relationIndices.data() )
      ;
    }

    bool              isValid(bool verboseOutput = false) const;


#ifdef AXOM_USE_BOOST
    RelationIterator  begin(SetPosition fromSetInd )
    {
      return (*this)[fromSetInd].begin();
    }

    RelationConstIterator begin(SetPosition fromSetInd ) const
    {
      return (*this)[fromSetInd].begin();
    }

    RelationIterator end(SetPosition fromSetInd)
    {
      return (*this)[fromSetInd].end();
    }

    RelationConstIterator end(SetPosition fromSetInd) const
    {
      return (*this)[fromSetInd].end();
    }


    RelationIteratorPair range(SetPosition fromSetInd)
    {
      return (*this)[fromSetInd].range();
    }

    RelationConstIteratorPair range(SetPosition fromSetInd) const
    {
      return (*this)[fromSetInd].range();
    }
#endif // AXOM_USE_BOOST


    bool                hasFromSet() const
    {
      return !EmptySetTraits<FromSetType>::isEmpty(m_fromSet);
    }
    FromSetType*        fromSet()       { return m_fromSet; }
    const FromSetType*  fromSet() const { return m_fromSet; }


    bool                hasToSet() const
    {
      return !EmptySetTraits<ToSetType>::isEmpty(m_toSet);
    }
    ToSetType*          toSet()       { return m_toSet; }
    const ToSetType*    toSet() const { return m_toSet; }


    void                setRelationData(SetPosition size, IndirectionBufferType* data)
    {
      m_relationIndices  = typename IndicesSet::SetBuilder()
          .size(size)
          .data(data);
    }

    const IndirectionBufferType* relationData() const
    {
      return m_relationIndices.data();
    }

    IndirectionBufferType* relationData()
    {
      return m_relationIndices.data();
    }

  private:
    FromSetType* m_fromSet;
    ToSetType* m_toSet;

    IndicesSet m_relationIndices;

  };


  /**
   * \brief Checks whether the relation is valid
   *
   * A relation is valid when:
   * * Its fromSet and toSet are not null
   * * The CardinalityPolicy is valid.
   *   This implies that for each element, pos, of the fromSet,
   *   it is valid to call rel.size(pos), rel.offset()
   *   It is also valid to call rel.totalSize()
   *
   *
   * @return True if the relation is valid, false otherwise
   */
  template<
    typename RelationCardinalityPolicy,
    typename RelationIndicesIndirectionPolicy,
    typename FromSetType,
    typename ToSetType>
  bool StaticRelation<RelationCardinalityPolicy,RelationIndicesIndirectionPolicy,FromSetType,ToSetType>::isValid(
    bool verboseOutput) const
  {
    std::stringstream errSstr;

    bool setsAreValid = true;
    bool cardinalityIsValid = true;
    bool relationdataIsValid = true;

    // Step 1: Check if the sets are valid
    bool isFromSetNull = (m_fromSet == AXOM_NULLPTR);
    bool isToSetNull   = (m_toSet == AXOM_NULLPTR);

    if(isFromSetNull || isToSetNull)
    {
      if(verboseOutput)
      {
        errSstr << "\n\t Static relations require both the fromSet and toSet to be non-null"
                << "\n\t -- fromSet was " << (isFromSetNull ? "" : " not ") << "null"
                << "\n\t -- toSet was " << (isToSetNull ? "" : " not ") << "null";
      }

      setsAreValid = false;
    }

    // Step 2: Check if the cardinality is valid
    if(setsAreValid)
    {
      if( !CardinalityPolicy::isValid(m_fromSet, verboseOutput) )
      {
        if(verboseOutput)
        {
          errSstr << "\n\t Invalid cardinality state.";
          //  TODO -- improve this message ;
        }
        cardinalityIsValid = false;
      }
    }

    // Step 3: Check if the relation data is valid
    if(cardinalityIsValid)
    {
      if( m_relationIndices.size() != this->totalSize() )
      {
        if(verboseOutput)
        {
          errSstr << "\n\t* relation indices has the wrong size."
                  << "\n\t-- from set size is: " << m_fromSet->size()
                  << "\n\t-- expected relation size: " << this->totalSize()
                  << "\n\t-- actual size: " << m_relationIndices.size()
          ;
        }
        relationdataIsValid = false;
      }

      if( !m_relationIndices.empty())
      {
        // Check that all begins offsets are in the right range
        // Specifically, they must be in the index space of m_relationIndices
        for(SetPosition pos = 0; pos < m_fromSet->size(); ++pos)
        {
          SetPosition off = this->offset(pos);
          if( !m_relationIndices.isValidIndex(off) && off != m_relationIndices.size() )
          {
            if(verboseOutput)
            {
              errSstr << "\n\t* Begin offset for index " << pos
                      << " was out of range."
                      << "\n\t-- value: " << this->offset(pos)
                      << " needs to be within range [0," << m_relationIndices.size() << "]"
              ;
            }
            relationdataIsValid = false;
          }
        }
      }

      // Check that all relation indices are in range for m_toSet
      for(SetPosition pos = 0; pos < m_relationIndices.size(); ++pos)
      {
        if (!m_toSet->isValidIndex ( m_relationIndices[pos]))
        {
          if(verboseOutput)
          {
            errSstr << "\n\t* Relation index was out of range."
                    << "\n\t-- value: " << m_relationIndices[pos]
                    << " needs to be in range [0," << m_toSet->size() << ")"
            ;
          }
          relationdataIsValid = false;
        }
      }
    }

    // We are done.  Output the messages if applicable and return
    bool bValid = setsAreValid && cardinalityIsValid && relationdataIsValid;

    if(verboseOutput && !bValid)
    {
      SLIC_DEBUG( errSstr.str() );
    }

    return bValid;
  }



} // end namespace slam
} // end namespace axom

#endif // SLAM_STATIC_RELATION_HPP_
