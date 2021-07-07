# SEIR Implementation

For the resolution of the SEIR system of differential equations we used 2 different methods:
The Euler Methods and the Runge Kutta Methods.


## Euler Methods

The Euler Methods is a numerical method to resolve differential equation by fixing an increment, preferably
small, which we can use to discretize the equations.
```math
\frac{dS}{dt} = -\beta \frac{S}{N} I \\[3mm]
\frac{dI}{dt} = \beta \frac{S}{N} I - \alpha E \\[3mm]
\frac{dE}{dt} = \alpha E - \gamma I \\[3mm]
\frac{dR}{dt} = \gamma I
```
Where we take as a fixed increment $`\Delta T = 1`$, which correspond to one day, and we obtain:
```math
S_{i+1} = S_i - \beta \frac{S_i}{N} I_i \\[3mm]
E_{i+1} = E_i + \beta \frac{S_i}{N} I_i - \alpha E_i \\[3mm]
I_{i+1} = I_i + \alpha E_i - \gamma I_i \\[3mm]
R_{i+1} = R_i + \gamma I_i \\
```
from which is easy to evaluate the system of equations.

### Notes

In the program we use floating point number for `S`,`E`,`I`,`R`. This can seem counter-intuitive as they 
represent numbers of person and should be represented by unsigned integer. But, by using integers the
value would be truncated and so we would add an approximation that will result in wrong results. As 
$`S + E + I + R != cost`$.