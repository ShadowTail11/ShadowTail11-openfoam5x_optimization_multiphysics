/*
 * This header implements the heaviside and PDE filter into the adjoint and cost function sensitivities
 */

Info << "\nCalculating Heaviside_rho sensitivity and implementing PDE filter\n" << endl;

// Normalize sensitivity functions by maximum value of respective functions
adjoint_sensitivity_init = adjoint_sensitivity_init / gMax(mag(adjoint_sensitivity_init.primitiveField()));
cost_sensitivity_init.primitiveFieldRef() = 1.0;

// Use Heaviside density filter to update sensitivities
if (opt > 1)
{
    for (i = 0; i < n_cells; i++)
    {
        if (gamma_densityfilter[i] <= eta5)
        {
            drho[i] = del * Foam::exp(-del * (1.0 - gamma_densityfilter[i] / eta5)) + Foam::exp(-del);
        }
        else
        {
            drho[i] = del * Foam::exp(-del * (gamma_densityfilter[i] - eta5) / (1.0 - eta5)) + Foam::exp(-del);
        }
    }
    adjoint_sensitivity_init = adjoint_sensitivity_init * drho;
    cost_sensitivity_init = cost_sensitivity_init * drho;
}

// Use PDE filter to update sensitivities
solve(fvm::laplacian(adjoint_sensitivity) - fvm::Sp(b, adjoint_sensitivity) + adjoint_sensitivity_init * b);
solve(fvm::laplacian(cost_sensitivity) - fvm::Sp(b, cost_sensitivity) + cost_sensitivity_init * b);

adjoint_sensitivity.primitiveFieldRef() = adjoint_sensitivity.primitiveFieldRef() * mesh.V() / gMax(mesh.V());
cost_sensitivity.primitiveFieldRef() = cost_sensitivity.primitiveFieldRef() * mesh.V() / gMax(mesh.V());
