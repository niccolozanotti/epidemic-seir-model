# SEIR Implementation

We took from section _2.2_ of [this][article] paper the system of ODEs to solve in order to get the evolution of a generic epidemic. \
So, with a suitable variable remampping based on our project request, and neglecting death rate(considered in the paper)
the system of equations to solve is the following:
```math
\frac{dS}{dt} = -\beta \frac{S}{N} I \\[4mm]
\frac{dE}{dt} = \beta \frac{S}{N} I - \alpha E \\[4mm]
\frac{dI}{dt} = \alpha E - \gamma I \\[4mm]
\frac{dR}{dt} = \gamma I
```
with the following meaning:
|   Symbol     |             Meaning                                                                                    |
| ------------ | -------------------------------------------------------------------------------------------------------|
| S            | Susceptibles individuals: individuals who can be infected                                              |
| E            | Exposed individuals: individuals who had an infectious contact and waiting for the symptoms to show    |
| I            | Infectious individuals spreading the disease                                                           |
| R            | Individuals no more susceptible to the disease either because they recovered or they died              |

We offer two different methods to determine the epidemic evolution:

## Euler Method
Euler Method is a 1th-order numerical method used to solve a differential equation by fixing an increment. \
In our case the variable on which our functions depend is time. We thus take as increment $`\Delta T = 1`$, which corresponds to one day(t being measured in days). \
What we obtain is the following:
```math
S_{i+1} = S_i - \beta \frac{S_i}{N} I_i \\[3mm]
E_{i+1} = E_i + \beta \frac{S_i}{N} I_i - \alpha E_i \\[3mm]
I_{i+1} = I_i + \alpha E_i - \gamma I_i \\[3mm]
R_{i+1} = R_i + \gamma I_i \\
```
from which is easy to evaluate the system of equations.
Since Euler Method is a first order Method, supposing to exactly know the real values of S,E,I,R it's possible to evauluate
the local truncation error(error commited for every step of the discretization), which corresponds to $`O(h^3)`$, 
$`h`$ being the fixed step size.
For small values of the step the local error is proportional to $`h^2`$.
In our program this method is implemented by `EulerSolver(const State&)` method of `SEIR` class.
## Runge-Kutta Method
The other supplied method is Runge Kutta Method, in particular 4th-order one, which guarantees a better approximation than Euler
Method with a local truncation error of $`O(h^5)`$ which means that by halving the step the result is an error reduction of
$`2^5`$.
The general recurrence formula for RK-4 Method is the following:
```math
y_{n+1} = y_{n} + \frac{h}{6}\big(k_1+2k_2+2k_3+k_4 \big) \\[3mm]
x_{n+1} = x_{n} + h                      
```
where
```math
k_1 = f(x,y)\\[3mm]
k_2 = f(x,y)\\[3mm]
k_3 = f(x,y)\\[3mm]
k_4 = f(x,y)\\[3mm]
x_{n+1} = x_{n} + h                      
```
### Notes

In the program we used floating point number for `S`,`E`,`I`,`R` state values. This can seem counter-intuitive as they 
represent numbers of person and should be represented by unsigned integer value. But, by using integers the
value would be truncated and so we would add an approximation that will result in wrong results. As 
$`S + E + I + R \neq cost`$.




[article]:https://iris.polito.it/retrieve/handle/11583/2835949/375491
