import numpy as np
from scipy.stats import norm


def black_scholes(S, K, T, r, sigma, q, option_type="Call"):
    """
    Calculate the Black-Scholes option price and Greeks for a dividend-paying stock.
    
    Parameters:
    S (float): Current price of the underlying asset
    K (float): Strike price of the option
    T (float): Time to expiration in years
    r (float): Risk-free interest rate (as a decimal)
    sigma (float): Volatility of the underlying asset (as a decimal)
    q (float): Dividend yield (as a decimal)
    option_type (str): "Call" or "Put"
    
    Returns:
    dict: Option price and Greeks
    """
    # No conversion needed since T is already in years

    # Calculate d1 and d2 with dividend yield
    d1 = (np.log(S / K) + (r - q + 0.5 * sigma ** 2) * T) / (sigma * np.sqrt(T))
    d2 = d1 - sigma * np.sqrt(T)

    # Calculate option price
    if option_type == "Call":
        price = S * np.exp(-q * T) * norm.cdf(d1) - K * \
            np.exp(-r * T) * norm.cdf(d2)
        delta = np.exp(-q * T) * norm.cdf(d1)
    else:  # Put
        price = K * np.exp(-r * T) * norm.cdf(-d2) - S * \
            np.exp(-q * T) * norm.cdf(-d1)
        delta = -np.exp(-q * T) * norm.cdf(-d1)

    # Calculate Greeks
    gamma = np.exp(-q * T) * norm.pdf(d1) / (S * sigma * np.sqrt(T))

    if option_type == "Call":
        theta = (-S * np.exp(-q * T) * norm.pdf(d1) * sigma / (2 * np.sqrt(T))
                 - r * K * np.exp(-r * T) * norm.cdf(d2)
                 + q * S * np.exp(-q * T) * norm.cdf(d1))
    else:  # Put
        theta = (-S * np.exp(-q * T) * norm.pdf(d1) * sigma / (2 * np.sqrt(T))
                 + r * K * np.exp(-r * T) * norm.cdf(-d2)
                 - q * S * np.exp(-q * T) * norm.cdf(-d1))

    vega = S * np.exp(-q * T) * norm.pdf(d1) * np.sqrt(T)

    if option_type == "Call":
        rho = K * T * np.exp(-r * T) * norm.cdf(d2)
    else:  # Put
        rho = -K * T * np.exp(-r * T) * norm.cdf(-d2)

    return {
        "price": price,
        "delta": delta,
        "gamma": gamma,
        "theta": theta,
        "vega": vega,
        "rho": rho
    }
