//
// The header file initialized parameters not initialized previously.
//

// Remove monitoring files from previous run
std::remove("Monitor_Time.txt");
std::remove("Monitor_PseudoDensityDelta.txt");
std::remove("Monitor_PowerLoss.txt");
std::remove("Monitor_PressureDrop.txt");
std::remove("Monitor_VolumeFraction.txt");
std::remove("PowerLoss_Reference.txt");

// Initialize variables
int opt(1);                                     // Optimization loop counter
int i;                                          // Initialize generic loop counter
scalar n_cells(gamma.size());                   // The number of cells in the domain
n_cells = static_cast<int>(n_cells);            // Force n_cells to be an integer
int opt_threshold = -1;                         // Updated to iteration count when merit value is within 2x of tolerance

scalar area = gSum(mesh.V());                   // Domain area
scalar len = std::pow(mesh.V()[0], 1.0 / 3);    // Characteristic length of domain
scalar n_ave = 30;                              // Number of samples to average over in moving average method

scalar vol_frac;                                // Volume fraction of domain
scalar cost_vol_frac;                           // Cost variable for volume fraction
scalar vol_frac_conv(0);                        // Percentage difference between volume fraction and target

scalar P_drop(0);                               // Pressure drop between inlet and outlet
scalar P_drop_prev;                             // Previous pressure drop
scalar dP_drop(0);                              // Percentage change in pressure drop

scalar power_loss;                              // Power loss of the fluid
scalar power_loss_ratio(0);                     // Power loss ratio of the fluid
scalar power_loss_ratio_prev;                   // Previous power loss ratio
scalar dpower_loss_ratio(0);                    // Percentage change in power loss ratio
scalar dpower_loss_ratio_ave(0);                // Moving average of percent change in power loss ratio

scalar gamma_switch(0);                         // Number of cells that switch between fluid and solid
scalar gamma_switch_prev;                       // Previous number of cells that switch between fluid and solid
scalar dgamma_switch(0);                        // Percentage change in number of cells that switch material
scalar dgamma_switch_ave(0);                    // Moving average of percent change in number of material switched cells

// Heaviside filter variables
double eta0, eta1, eta5 = 0, del = 0.1, y0, y5;
b.value()= 1.0 / (r_filter * len / 3.464) / (r_filter * len / 3.464);

// Initialize Method of Moving Asymptotes (MMA)
int m_constraints = 1;                                      // Number of constraints
MMA mma(n_cells, m_constraints);                            // Set MMA optimization size
mma.asyminit = mma_init;                                    // Set initial movement
mma.asymdec = mma_dec;                                      // Set decrement size
mma.asyminc = mma_inc;                                      // Set increment size
mma.raa0 = raa0;                                            // Set smoothing factor
mma.movelimit = mma_limit;                                  // Set movement limit
std::vector<double> xmma(n_cells);                          // Vector that stores optimized gamma
std::vector<double> dfdx(n_cells);                          // Vector that stores sensitivity function
std::vector<double> gx(m_constraints);                      // Vector that stores constraint values
std::vector<std::vector<double>> dgdx(m_constraints);       // Vector that stores constrain functions
for (int i = 0; i < m_constraints; i++)                     // Resize constraint vector to size of domain
{
    dgdx[i].resize(n_cells);
}
