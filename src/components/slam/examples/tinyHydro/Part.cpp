/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */


#include "Part.hpp"

#include <sstream>
#include <string>
#include <cstring>

#include "slic/slic.hpp"

namespace tinyHydro {


//----------------------------------------------
  Part::Part(const int * zoneList, int nzones, double gamma)
      : gamma(gamma)
  {
    SLIC_DEBUG("in Part c'tor");

    zones            = ZoneSubset(nzones);
    // UGLY HACK to allocate a data buffer, copy in the data and set as the zonal indirection field data... must improve!
    std::vector<int>& ptr = DataRegistry::setRegistry.addNamelessField( &zones).data();
    std::memcpy(&ptr[0], zoneList, sizeof(int) * nzones);
    zones.data()     = &ptr;

    density          = ZonalScalarField(&zones);
    energyPerMass    = ZonalScalarField(&zones);
    volumeFraction   = ZonalScalarField(&zones);
  }
//----------------------------------------------
  Part::Part(const std::vector<int>& zoneList, double gamma)
      : gamma(gamma)
  {
    SLIC_DEBUG("in Part c'tor");

    /// UGLY hack to place zoneList data in the field registry and set the data of zones to this...
//   zones = ZoneSubset::SetBuilder()
//               .size( zoneList.size() )
//               .data( &zoneList );
//   setRegistry.addNamelessField( &zones).data().swap( zoneList);

    zones            = ZoneSubset( zoneList.size() );
    std::vector<int>& ptr = DataRegistry::setRegistry.addNamelessField( &zones).data();
    std::memcpy(&ptr[0], &zoneList[0], sizeof(int) * zones.size());
    zones.data()     = &zoneList;

    density          = ZonalScalarField(&zones);
    energyPerMass    = ZonalScalarField(&zones);
    volumeFraction   = ZonalScalarField(&zones);
  }

//----------------------------------------------
// Copy
  Part::Part(const Part & arg)
      :    gamma(arg.gamma)
  {
    // printf("in Part::copy \n");

    zones           = ZoneSubset(arg.zones.size());
    // UGLY HACK
    std::vector<int>& ptr = DataRegistry::setRegistry.addNamelessField( &zones).data();
    std::memcpy(&ptr[0], &arg.zones[0], sizeof(int) * zones.size());
    zones.data()    = &ptr;

    // copy field data
    density         = ZonalScalarField(arg.density);
    energyPerMass   = ZonalScalarField(arg.energyPerMass);
    volumeFraction  = ZonalScalarField(arg.volumeFraction);
  }

//----------------------------------------------
// Assignment
  Part & Part::operator=(const Part & rhs)
  {
    // printf("Part::assignment \n");

    // check for self-assignment
    if (this == &rhs)
      return *this;

    // copy over the gamma value
    gamma = rhs.gamma;

    zones            = ZoneSubset(rhs.zones.size());
    // UGLY HACK
    std::vector<int>& ptr = DataRegistry::setRegistry.addNamelessField( &zones).data();
    std::memcpy(&ptr[0], &rhs.zones[0], sizeof(int) * zones.size());
    zones.data()     = &ptr;

    // copy field data
    density          = ZonalScalarField(rhs.density);
    energyPerMass    = ZonalScalarField(rhs.energyPerMass);
    volumeFraction   = ZonalScalarField(rhs.volumeFraction);

    return *this;
  }

//----------------------------------------------
// addition in place
  Part & Part::operator+=(const Part & rhs)
  {
    // printf("in Part::operator+= \n");
    SLIC_ASSERT(numZones() == rhs.numZones() );

    const int nZones = numZones();
    for (int i = 0; i < nZones; i++)
    {
      density[i]        += rhs.density[i];
      energyPerMass[i]  += rhs.energyPerMass[i];
      volumeFraction[i] += rhs.volumeFraction[i];
    }

    return *this;
  }
//----------------------------------------------
// scalar multiply in place
  Part & Part::operator*=(const double s)
  {
    // printf("in Part::operator*= \n");

    const int nZones = numZones();

    for (int i = 0; i < nZones; i++)
    {
      density[i]        *= s;
      energyPerMass[i]  *= s;
      volumeFraction[i] *= s;
    }

    return *this;
  }
//----------------------------------------------
//----------------------------------------------


  void Part::dumpPart()
  {
    SLIC_INFO("Part has " << numZones() << " zones" );

    std::stringstream zoneStr;
    zoneStr << "\nZones:";
    for(int i = 0; i< numZones(); ++i)
    {
      zoneStr << "\n\t Zone " << i << " -- idx " << zones[i];
    }
    SLIC_INFO( zoneStr.str() );
  }

} // end namespace tinyHydro