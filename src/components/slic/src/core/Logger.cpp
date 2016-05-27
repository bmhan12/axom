/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */

/*!
 *******************************************************************************
 * \file Logger.cpp
 *
 * \date May 7, 2015
 * \author George Zagaris (zagaris2@llnl.gov)
 *
 *******************************************************************************
 */

#include "Logger.hpp"

#include "LogStream.hpp"

#include "common/CommonTypes.hpp"

// C/C++ includes
#include <iostream> // for std::cout, std::cerr

namespace asctoolkit {

namespace slic {

Logger* Logger::s_Logger = ATK_NULLPTR;
std::map< std::string, Logger* > Logger::s_loggers;

//------------------------------------------------------------------------------
Logger::Logger()
{
  // by default, all message streams are disabled
  for ( int i=0; i < message::Num_Levels; ++i ) {

    m_isEnabled[ i ] = false;

  }


}

//------------------------------------------------------------------------------
Logger::Logger(const std::string& name) :
        m_name( name )
{
  // by default, all message streams are disabled
  for ( int i=0; i < message::Num_Levels; ++i ) {

    m_isEnabled[ i ] = false;

  }

}

//------------------------------------------------------------------------------
Logger::~Logger()
{
  std::map< LogStream*, LogStream* >::iterator it =
      m_streamObjectsManager.begin();
  for ( ; it != m_streamObjectsManager.end(); ++it ) {
    delete it->second;
  } // END for all logStreams

  for ( int level=message::Fatal; level < message::Num_Levels; ++level ) {

    m_logStreams[ level ].clear();

  } // END for all levels

}

//------------------------------------------------------------------------------
void Logger::setLoggingMsgLevel( message::Level level )
{
  for ( int i=0; i < message::Num_Levels; ++i ) {
    m_isEnabled[ i ] = (i<= level) ? true : false;
  }

}

//------------------------------------------------------------------------------
void Logger::addStreamToMsgLevel( LogStream* ls, message::Level level,
                               bool pass_ownership )
{
  if ( ls == ATK_NULLPTR ) {

    std::cerr << "WARNING: supplied log stream is NULL!\n";
    return;

  }

  m_logStreams[ level ].push_back( ls );

  if ( pass_ownership ) {

    m_streamObjectsManager[ ls ] = ls;

  }

}

//------------------------------------------------------------------------------
void Logger::addStreamToAllMsgLevels( LogStream* ls )
{
  if ( ls == ATK_NULLPTR ) {

    std::cerr << "WARNING: supplied log stream is NULL!\n";
    return;

  }

  for ( int level=message::Fatal; level < message::Num_Levels; ++level ) {

    this->addStreamToMsgLevel( ls, static_cast<message::Level>( level ) );

  } // END for all levels

}

//------------------------------------------------------------------------------
int Logger::getNumStreamsAtMsgLevel( message::Level level )
{
  return m_logStreams[ level ].size( );
}

//------------------------------------------------------------------------------
LogStream* Logger::getStream( message::Level level , int i )
{
  if ( i < 0 || i >= static_cast<int>(m_logStreams[ level ].size()) ) {
    std::cerr << "ERROR: stream index is out-of-bounds!\n";
    return ATK_NULLPTR;
  }

  return m_logStreams[ level ][ i ];
}

//------------------------------------------------------------------------------
void Logger::logMessage( message::Level level,
                         const std::string& message,
                         bool filter_duplicates )
{
  this->logMessage(
      level,message,MSG_IGNORE_TAG,MSG_IGNORE_FILE,MSG_IGNORE_LINE,
      filter_duplicates );
}

//------------------------------------------------------------------------------
void Logger::logMessage( message::Level level,
                         const std::string& message,
                         const std::string& tagName,
                         bool filter_duplicates )
{
  this->logMessage( level,message,tagName,MSG_IGNORE_FILE,MSG_IGNORE_LINE,
                    filter_duplicates );
}

//------------------------------------------------------------------------------
void Logger::logMessage( message::Level level,
                         const std::string& message,
                         const std::string& fileName,
                         int line,
                         bool filter_duplicates )
{
  this->logMessage( level, message, MSG_IGNORE_TAG, fileName, line,
                    filter_duplicates );
}

//------------------------------------------------------------------------------
void Logger::logMessage( message::Level level,
                         const std::string& message,
                         const std::string& tagName,
                         const std::string& fileName,
                         int line,
                         bool filter_duplicates )
{
  if ( m_isEnabled[ level ]==false  ) {

    /* short-circuit */
    return;

  } // END if

  unsigned nstreams = m_logStreams[ level ].size();
  for ( unsigned istream=0; istream < nstreams; ++istream ) {

    m_logStreams[ level ][ istream ]->append(
                              level,message,tagName,fileName,line,
                              filter_duplicates );

  } // END for all streams

}

//------------------------------------------------------------------------------
void Logger::flushStreams()
{
  for ( int level=message::Fatal; level < message::Num_Levels; ++level ) {

    unsigned nstreams = m_logStreams[ level ].size();
    for ( unsigned istream=0; istream < nstreams; ++istream ) {
      m_logStreams[ level ][ istream ]->flush( );

    } // END for all streams

  } // END for all levels

}

//------------------------------------------------------------------------------
void Logger::pushStreams()
{
  for ( int level=message::Fatal; level < message::Num_Levels; ++level ) {

    unsigned nstreams = m_logStreams[ level ].size();
    for ( unsigned istream=0; istream < nstreams; ++istream ) {
      m_logStreams[ level ][ istream ]->push( );

    } // END for all streams

  } // END for all levels

}

//------------------------------------------------------------------------------
//                Static Method Implementations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Logger::initialize()
{
  Logger::createLogger("root");
  Logger::activateLogger("root");
}

//------------------------------------------------------------------------------
bool Logger::createLogger( const std::string& name, char imask )
{

  if ( s_loggers.find( name ) != s_loggers.end() ) {
    std::cerr << "ERROR: " << name << " logger is duplicated!\n";
    return false;
  }

  s_loggers[ name ] = new Logger( name );

  if ( imask == inherit::nothing ) {
    /* short-circuit */
    return true;
  } // END if inherit nothing

  Logger* rootLogger = Logger::getRootLogger();
  if ( rootLogger == ATK_NULLPTR ) {
    std::cerr << "ERROR: no root logger found!\n";
    return false;
  }

  for ( int level=message::Fatal; level < message::Num_Levels; ++level ) {

    message::Level current_level = static_cast< message::Level >( level );

    int nstreams = rootLogger->getNumStreamsAtMsgLevel( current_level );
    if ( nstreams == 0 ) {
      continue;
    }

    if ( imask & inherit::masks[ level ] ) {

      for ( int istream=0; istream < nstreams; ++istream ) {

        s_loggers[ name ]->addStreamToMsgLevel(
            rootLogger->getStream(current_level,istream),
            current_level,
            /* pass_ownership */ false );

      } // END for all streams at this level

    } // END if inherit streams at the given level

  } // END for all message levels


  return true;
}

//------------------------------------------------------------------------------
bool Logger::activateLogger( const std::string& name )
{
  if ( s_loggers.find( name ) != s_loggers.end() ) {
    s_Logger = s_loggers[ name ];
    return true;
  }

  return false;
}

//------------------------------------------------------------------------------
void Logger::finalize()
{
  std::map< std::string, Logger* >::iterator it;
  for ( it=s_loggers.begin(); it != s_loggers.end(); ++it ) {
    it->second->flushStreams();
  }

  for ( it=s_loggers.begin(); it != s_loggers.end(); ++it ) {
    delete it->second;
  }

  s_loggers.clear();
  s_Logger = ATK_NULLPTR;
}

//------------------------------------------------------------------------------
std::string Logger::getActiveLoggerName()
{
  return s_Logger->getName();
}

//------------------------------------------------------------------------------
Logger* Logger::getActiveLogger()
{
  return s_Logger;
}

//------------------------------------------------------------------------------
Logger* Logger::getRootLogger()
{
  if ( s_loggers.find( "root" ) == s_loggers.end() ) {
    // no root logger
    return ATK_NULLPTR;
  }

  return ( s_loggers[ "root" ] );
}

} /* namespace slic */

} /* namespace asctoolkit */