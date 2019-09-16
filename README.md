# Equity-Option-Pricing

#This module performs Equity Vanilla Option Pricing, generates greeks and performs volatility smile fitting based on user inputted Vol/Strike grid.

________________________________________________________________________________________________________________________________________

Function 1: Vanilla Option Pricing
________________________________________________________________________________________________________________________________________

The module involves implementation of Black Scholes pricing for user inputted Vanilla Equity Option trade economics - Spot Price (S), Strike Price (K), Volatility (v), Risk Free Rate of Return (r), Time to Expiry (T) and Option Type ('P' for Put, 'C' for Call). The option premium is calculated as per below.

Call Option Price = S*N(d1)-K*exp(-r*t)*N(d2)
Put Option Price = K*exp(-r*t)*N(-d2)-S*N(-d1)

where d1=(ln(S/K)+(r+V*V/2)*T)/v*sqrt(T)

      d2=(ln(S/K)+(r-V*V/2)*T)/v*sqrt(T) = d1-v*sqrt(T)

      N(x) represents cumulative probability function for a standardised normal distribution
      
The code uses an approximation of the cumulative probability function for the standard normal distribution.

Please note that volatility and risk free rate of return should be input as fractions (Ex: 0.05 for 5%).

The code calculates the option premium and prints on console.

________________________________________________________________________________________________________________________________________

Function 2: Vanilla Option Greeks Calculation
________________________________________________________________________________________________________________________________________

The module calculates the following option greeks.

1> Delta: Change in option premium with respect to unit change in Spot Price (S)
2> Gamma: Change in option delta with respect to unit change in Spot Price (S)
3> Vega : Change in option premium with respect to unit change in Volatility (v)
4> Theta: Change in option premium with respect to unit change in time to expiry (T)
5> Rho  : Change in option premium with respect to unit change in risk free rate of return (r)

The module calculates the above greeks using two different tweak methodologies

1> Absolute Tweak Methodology: It tweaks the underlying parameters used in option pricing - S,K,r,T,v using a fixed amount. For ex: 1% change in Volatility. If the current market volatility is 20%, it gets tweaked to 20%+1% = 21% to calculate the vega impact.

2> Relative Tweak Methodology: It tweaks the underlying parameters used in option pricing - S,K,r,T,v by a relative amount. For ex: 1% relative change in Volatility. If the current market volatility is 20%, it gets tweaked to 20%(1+1%) = 20.2% to calculate the vega impact.

The user can reference the greeks as per the market accepted practise for tweaking during greeks calculation.

________________________________________________________________________________________________________________________________________

Function 3: Volatility Smile Fitting
________________________________________________________________________________________________________________________________________











