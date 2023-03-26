//
// This header file reads user-defined properties from the optimizationProperties file in the application's
// 'constant' directory
//

Info << "Reading optimization properties\n" << endl;

// Establishing link to optimizationProperties file
IOdictionary optimizationProperties(

        IOobject(

                "optimizationProperties",
                runTime.constant(),
                mesh,
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE
        )
);

// Domain definition
scalar solid_area(readScalar(optimizationProperties.lookup("solid_area")));             // Is solid region set? (1=yes)
scalar fluid_area(readScalar(optimizationProperties.lookup("fluid_area")));             // Is fluid region used? (1=yes)
scalar test_area(readScalar(optimizationProperties.lookup("test_area")));               // Is test region used? (1=yes)
scalar geo_dim(readScalar(optimizationProperties.lookup("geo_dim")));                   // Dimensions of solver (2D vs. 3D)

// Optimization properties
scalar mma_init(readScalar(optimizationProperties.lookup("mma_init")));                 // Initial optimization increment
scalar mma_dec(readScalar(optimizationProperties.lookup("mma_dec")));                   // Optimization decrement
scalar mma_inc(readScalar(optimizationProperties.lookup("mma_inc")));                   // Optimization increment
scalar mma_limit(readScalar(optimizationProperties.lookup("mma_limit")));               // Optimization limit
scalar raa0(readScalar(optimizationProperties.lookup("raa0")));                         // MMA smoothing parameter
scalar r_filter_scalar(readScalar(optimizationProperties.lookup("r_filter")));          // Density filter radius
int r_filter = r_filter_scalar;
dimensionedScalar b("b", dimensionSet(0,-2,0,0,0,0,0),1.0);                             // Filter radius of the PDE filter

// Cost function constraints
scalar set_vol_frac(readScalar(optimizationProperties.lookup("set_vol_frac")));         // Target volume fraction
scalar set_power_ratio(readScalar(optimizationProperties.lookup("set_power_ratio")));   // Target power loss ratio
scalar power_loss_ref(readScalar(optimizationProperties.lookup("power_loss_ref")));     // Power loss reference

// Optimization weights
scalar weight_TU(readScalar(optimizationProperties.lookup("weight_TU")));               // Sensitivity weight for thermal velocity term
scalar weight_TK(readScalar(optimizationProperties.lookup("weight_TK")));               // Sensitivity weight for thermal conductivity term
scalar weight_TQ(readScalar(optimizationProperties.lookup("weight_TQ")));               // Sensitivity weight for heat generation term
scalar weight_TT(readScalar(optimizationProperties.lookup("weight_TT")));               // Sensitivity weight for thermal diffusivity term
scalar weight_T(readScalar(optimizationProperties.lookup("weight_T")));                 // Sensitivity weight for thermal term
scalar weight_U(readScalar(optimizationProperties.lookup("weight_U")));                 // Sensitivity weight for velocity term

// Adjoint unit correctors (used because adjoint variables have inverse units of parent, but OpenFOAM uses same units)
dimensionedScalar unit_correct("unit_correct", dimensionSet(0,2,-2,-2,0,0,0),1.0);      // Adjoint (U/T)^2 unit correction
dimensionedScalar unit_correct_U("unit_correct_U", dimensionSet(0,-1,1,0,0,0,0),1.0);   // Adjoint U unit correction
dimensionedScalar unit_correct_T("unit_correct_T", dimensionSet(0,0,0,-1,0,0,0),1.0);   // Adjoint T unit correction

// Convergence controls
scalar merit_tol(readScalar(optimizationProperties.lookup("merit_tol")));               // Convergence tolerance for merit parameters
scalar gamma_tol(readScalar(optimizationProperties.lookup("gamma_tol")));               // Convergence tolerance for pseudo density gamma