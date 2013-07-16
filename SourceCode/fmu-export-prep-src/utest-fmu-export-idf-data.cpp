//--- Unit test for fmu-export-idf-data.cpp.
//
/// \author David Lorenzetti,
///         Lawrence Berkeley National Laboratory,
///         dmlorenzetti@lbl.gov
/// \brief  Unit test for fmu-export-idf-data.cpp.


//--- Includes.
//
#include <assert.h>

#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

#include "fmu-export-idf-data.h"

#include "../read-ep-file-src/ep-idd-map.h"
#include "../read-ep-file-src/fileReaderData.h"
#include "../read-ep-file-src/fileReaderDictionary.h"


//--- Main driver.
//
//   Collect data needed to prepare an EnergyPlus IDF file to be exported as an
// FMU.  Echo the data to stdout.
//
int main(int argc, const char* argv[]) {
  //
  // Check arguments.
  if( 3 != argc ){
    cout << "Error: missing filename\nUsage: " << argv[0] << "  <name of IDD file to guide parsing>  <name of IDF file to parse>\n";
    return(1);
  }
  //
  // Set up data dictionary.
  fileReaderDictionary frIdd(argv[1]);
  frIdd.open();
  iddMap idd;
  frIdd.getMap(idd);
  //
  // Check data dictionary.
  fmuExportIdfData fmuIdfData;
  string errStr;
  if( ! fmuIdfData.haveValidIDD(idd, errStr) )
    {
    cout << "Incompatible IDD file " << argv[1] << endl << errStr << endl;
    return( EXIT_FAILURE );
    }
  //
  // Initialize input data file.
  fileReaderData frIdf(argv[2], IDF_DELIMITERS_ENTRY, IDF_DELIMITERS_SECTION);
  //
  // Attach an external reporting function.
  //   Not done for this unit test.
  // frIdf.attachErrorFcn(reportInputError);
  //
  frIdf.open();
  //
  // Read IDF file for data of interest.
  const int failLine = fmuIdfData.populateFromIDF(frIdf);
  //
  // Check read.
  if( 0 < failLine )
    {
    cout << "Error detected while reading IDF file " << argv[2] << ", at line #" << failLine << endl;
    return( EXIT_FAILURE );
    }
  assert( 0 == failLine );
  //
  // Finish checking data extracted from IDF file.
  if( ! fmuIdfData.check() )
    {
    cout << "Error detected after finished reading IDF file " << argv[2] << endl;
    return( EXIT_FAILURE );
    }
  //
  // Echo collected data.
  int datCt, idx;
  cout << "EnergyPlus data needed to prepare an FMU, as read from IDF file " << argv[2] << ":" << endl << endl;
  //
  // To actuator.
  datCt = (int)fmuIdfData._toActuator_idfLineNo.size();
  assert( (int)fmuIdfData._toActuator_epName.size() == datCt );
  assert( (int)fmuIdfData._toActuator_fmuVarName.size() == datCt );
  assert( (int)fmuIdfData._toActuator_initValue.size() == datCt );
  if( 0 < datCt )
    {
    cout << "-- _toActuator_idfLineNo, _toActuator_epName, _toActuator_fmuVarName, _toActuator_initValue:" << endl;
    for( idx=0; idx<datCt; ++idx )
      {
      cout << fmuIdfData._toActuator_idfLineNo[idx] << ", " <<
        fmuIdfData._toActuator_epName[idx] << ", " <<
        fmuIdfData._toActuator_fmuVarName[idx] << ", " <<
        fmuIdfData._toActuator_initValue[idx] << endl;
      }
    cout << endl;
    }
  //
  // To schedule.
  datCt = (int)fmuIdfData._toSched_idfLineNo.size();
  assert( (int)fmuIdfData._toSched_epSchedName.size() == datCt );
  assert( (int)fmuIdfData._toSched_fmuVarName.size() == datCt );
  assert( (int)fmuIdfData._toSched_initValue.size() == datCt );
  if( 0 < datCt )
    {
    cout << "-- _toSched_idfLineNo, _toSched_epSchedName, _toSched_fmuVarName, _toSched_initValue:" << endl;
    for( idx=0; idx<datCt; ++idx )
      {
      cout << fmuIdfData._toSched_idfLineNo[idx] << ", " <<
        fmuIdfData._toSched_epSchedName[idx] << ", " <<
        fmuIdfData._toSched_fmuVarName[idx] << ", " <<
        fmuIdfData._toSched_initValue[idx] << endl;
      }
    cout << endl;
    }
  //
  // To variable.
  datCt = (int)fmuIdfData._toVar_idfLineNo.size();
  assert( (int)fmuIdfData._toVar_epName.size() == datCt );
  assert( (int)fmuIdfData._toVar_fmuVarName.size() == datCt );
  assert( (int)fmuIdfData._toVar_initValue.size() == datCt );
  if( 0 < datCt )
    {
    cout << "-- _toVar_idfLineNo, _toVar_epName, _toVar_fmuVarName, _toVar_initValue:" << endl;
    for( idx=0; idx<datCt; ++idx )
      {
      cout << fmuIdfData._toVar_idfLineNo[idx] << ", " <<
        fmuIdfData._toVar_epName[idx] << ", " <<
        fmuIdfData._toVar_fmuVarName[idx] << ", " <<
        fmuIdfData._toVar_initValue[idx] << endl;
      }
    cout << endl;
    }
  //
  // From variable.
  datCt = (int)fmuIdfData._fromVar_idfLineNo.size();
  assert( (int)fmuIdfData._fromVar_epKeyName.size() == datCt );
  assert( (int)fmuIdfData._fromVar_epVarName.size() == datCt );
  assert( (int)fmuIdfData._fromVar_fmuVarName.size() == datCt );
  if( 0 < datCt )
    {
    cout << "-- _fromVar_idfLineNo, _fromVar_epKeyName, _fromVar_epVarName, _fromVar_fmuVarName:" << endl;
    for( idx=0; idx<datCt; ++idx )
      {
      cout << fmuIdfData._fromVar_idfLineNo[idx] << ", " <<
        fmuIdfData._fromVar_epKeyName[idx] << ", " <<
        fmuIdfData._fromVar_epVarName[idx] << ", " <<
        fmuIdfData._fromVar_fmuVarName[idx] << endl;
      }
    cout << endl;
    }
  //
  return(0);
}  // End fcn main().