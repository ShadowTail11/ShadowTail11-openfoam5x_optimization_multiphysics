/*
 * This function implements the primal equation for heat transfer by means of the energy equation.
 * This equation assumes that the thermal diffusion is only happening in the fluid region (gamma=1)
 * and that there is heat generation Q in the solid region only (gamma=0).
 */

{
    // Implement thermal solver to calculate temperature T
    Info << "\nSolve for T\n" << endl;

    // Define primal energy equation for test condition
    fvScalarMatrix TEqn
    (
            fvm::div(phi, T)                    // Thermal diffusivity
            - fvm::laplacian(alpha_T_eff, T)    // Thermal conduction term
            ==
            fvOptions(T) + q_gen * (1 - gamma)  // Heat generation in solid region
//            fvOptions(T) + q_gen              // Heat generation in whole domain
    );

    // Relax and solve for temperature T_test
    TEqn.relax();
    fvOptions.constrain(TEqn);
    TEqn.solve();
    fvOptions.correct(T);

    // Correct temperature so that minimum does not go below T_min
    forAll(T, i)
    {
        T[i] = Foam::max(T[i], T_min.value());
    }
}
