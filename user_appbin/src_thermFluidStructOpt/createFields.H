//
// This header file manages the calculation and creation of fields.
//

#include "readOptProperties.h"          // Reads user-defined optimization properties
#include "readTransProperties.H"        // Reads transport properties
#include "readThermProperties.H"        // Reads thermal properties
#include "readMechanicalProperties.H"   // Reads mechanical properties
#include "setDensityFields.h"           // Sets fields related to zones and pseudo density gamma
#include "setTransFields.h"             // Sets fields related to transport properties
#include "setThermFields.h"             // Sets fields related to thermal properties
#include "setStructuralFields.h"        // Sets fields related to mechanical properties
#include "setSensFields.h"              // Sets fields related to adjoint sensitivity
