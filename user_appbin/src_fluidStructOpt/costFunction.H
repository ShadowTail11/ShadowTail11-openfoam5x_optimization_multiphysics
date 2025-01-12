/*
 * This header calculates cost functions and convergence tracking variables.
 */

{
    Info<< "\nCalculating the monitoring & cost function variables" << endl;

    //// Pressure Drop (used for monitoring)
    P_drop_prev = P_drop;
    P_drop = (sum(phi.boundaryField()[conPatchList[1]] * p.boundaryField()[conPatchList[1]])
              / sum(phi.boundaryField()[conPatchList[1]])
              -sum(phi.boundaryField()[conPatchList[0]] * p.boundaryField()[conPatchList[0]])
               / sum(phi.boundaryField()[conPatchList[0]])) * rho_fluid.value();

    Info << "\n--> Pressure drop between inlet and outlet is: " << P_drop << endl;

    dP_drop = abs((P_drop - P_drop_prev) / (P_drop + SMALL));

    Info << "\n--> Percentage change in pressure drop is: " << dP_drop * 100 << "%" << endl;

    //// Volume Fraction (used for monitoring, cost, & convergence)
    vol_frac = fvc::domainIntegrate(gamma).value() / area;

    Info << "\n--> Volume fraction is: " << vol_frac << endl;

    cost_vol_frac = vol_frac - set_vol_frac;
    vol_frac_conv = abs(cost_vol_frac / set_vol_frac);

    Info << "\n--> Percentage difference between volume fraction and target is: " << vol_frac_conv * 100 << "%" << endl;

    //// Power loss ratio (used for monitoring, cost, & convergence)
    power_loss = 0;
    for(i = 0; i < nObjPatch; i++)
    {
    power_loss = power_loss - sum(
            phi.boundaryField()[conPatchList[i]] * (p.boundaryField()[conPatchList[i]]
            + 0.5 * magSqr(U.boundaryField()[conPatchList[i]])));
    }

    power_loss_ratio_prev = power_loss_ratio;
    power_loss_ratio = power_loss / power_loss_ref;

    // Divide the power loss ratio by the characteristic length V^1/3 if 2D
    if(geo_dim == 2)
    {
        power_loss_ratio = power_loss_ratio / len;
    }

    Info << "\n--> Power dissipation ratio is: " << power_loss_ratio << endl;

    dpower_loss_ratio = abs((power_loss_ratio - power_loss_ratio_prev) / (power_loss_ratio + SMALL));
    dpower_loss_ratio_ave = (1 - 1 / n_ave) * dpower_loss_ratio_ave + dpower_loss_ratio / n_ave;

    Info << "\n--> Percentage change in power loss ratio is: " << dpower_loss_ratio_ave * 100 << "%\n" << endl;

    //// Compliance (used for monitoring, cost, & convergence)

    gradD = fvc::grad(D);
    sigmaD = mu * twoSymm(gradD) + (lambda * I) * tr(gradD);
    volScalarField Energy(0.25 * (gradD + gradD.T()) && sigmaD);

    compliance_ratio = fvc::domainIntegrate(Energy).value() / compliance_ref;

    Info << "\n--> Compliance ratio is: " << compliance_ratio << endl;

    cost_compliance = compliance_ratio / Foam::max(set_compliance * 3 - opt * 0.2, set_compliance) - 1.0;

    compliance_conv = abs(compliance_ratio / set_compliance - 1);

    Info << "\n--> Percentage difference between compliance ratio and target is: " << compliance_conv * 100 << "%\n" << endl;
}

