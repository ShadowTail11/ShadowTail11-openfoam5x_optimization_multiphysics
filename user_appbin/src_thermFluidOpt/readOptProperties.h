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
scalar r_filter(round(readScalar(optimizationProperties.lookup("r_filter"))));          // Density filter radius
dimensionedScalar b("b", dimensionSet(0,-2,0,0,0,0,0),1.0);                             // Filter radius of the PDE filter

// Cost function constraints
scalar vol_frac_ref(readScalar(optimizationProperties.lookup("vol_frac_ref")));         // Reference volume fraction
scalar vol_frac_ratio(readScalar(optimizationProperties.lookup("vol_frac_ratio")));     // Ratio used to calculate the set volume fraction
scalar set_power_ratio(readScalar(optimizationProperties.lookup("set_power_ratio")));   // Target power loss ratio
scalar power_loss_ref(readScalar(optimizationProperties.lookup("power_loss_ref")));     // Power loss reference
scalar set_vol_frac = vol_frac_ref * vol_frac_ratio;                                    // Target volume fraction

// Optimization weights
scalar weight_sens_TU(readScalar(optimizationProperties.lookup("weight_sens_TU")));     // Sensitivity weight for thermal velocity term
scalar weight_sens_TK(readScalar(optimizationProperties.lookup("weight_sens_TK")));     // Sensitivity weight for thermal conductivity term
scalar weight_sens_TQ(readScalar(optimizationProperties.lookup("weight_sens_TQ")));     // Sensitivity weight for heat generation term
scalar weight_sens_TT(readScalar(optimizationProperties.lookup("weight_sens_TT")));     // Sensitivity weight for thermal diffusivity term
scalar weight_sens_T(readScalar(optimizationProperties.lookup("weight_sens_T")));       // Sensitivity weight for thermal term
scalar weight_sens_U(readScalar(optimizationProperties.lookup("weight_sens_U")));       // Sensitivity weight for velocity term
scalar weight_sens_vf(readScalar(optimizationProperties.lookup("weight_sens_vf")));     // Sensitivity weight for volume fraction term
scalar weight_sens_tot(readScalar(optimizationProperties.lookup("weight_sens_tot")));   // Sensitivity weight for all terms

// Adjoint unit correctors (used because adjoint variables have inverse units of parent, but OpenFOAM uses same units)
dimensionedScalar unit_correct("unit_correct", dimensionSet(0,2,-2,-2,0,0,0),1.0);      // Adjoint (U/T)^2 unit correction
dimensionedScalar unit_correct_U("unit_correct_U", dimensionSet(0,-1,1,0,0,0,0),1.0);   // Adjoint U unit correction
dimensionedScalar unit_correct_T("unit_correct_T", dimensionSet(0,0,0,-1,0,0,0),1.0);   // Adjoint T unit correction

// Convergence controls
scalar conv_tol_flow(readScalar(optimizationProperties.lookup("conv_tol_flow")));       // Convergence threshold for fluid solver
int n_flow_solve(round(readScalar(optimizationProperties.lookup("n_flow_solve"))));     // Number of iterations used for solver per optimization loop
scalar merit_tol(readScalar(optimizationProperties.lookup("merit_tol")));               // Convergence tolerance for merit parameters
scalar gamma_tol(readScalar(optimizationProperties.lookup("gamma_tol")));               // Convergence tolerance for pseudo density gamma
