import streamlit as st
import matplotlib.pyplot as plt
import numpy as np
from black_scholes import black_scholes

# Set page configuration
st.set_page_config(
    page_title="Black-Scholes Option Price Calculator", layout="wide")

# Inject custom CSS to center the title and prevent wrapping
st.markdown(
    """
    <style>
    .title {
        text-align: center;
        white-space: nowrap;  /* Prevents the title from wrapping to the next line */
        font-size: 36px;     /* Adjust font size if needed */
    }
    </style>
    """,
    unsafe_allow_html=True
)

# Title (centered and on a single line)
st.markdown('<h1 class="title">Black-Scholes Option Price Calculator</h1>', unsafe_allow_html=True)

# Sidebar for inputs
st.sidebar.header("Black-Scholes Parameters")

# Input fields with adjusted max values and percentage inputs
risk_free_rate_percent = st.sidebar.number_input(
    "Risk-Free Rate (%)", min_value=0.0, max_value=20.0, value=3.0, step=0.1, format="%.1f",
    help="Enter the risk-free rate as a percentage (e.g., 3 for 3%).")
underlying_price = st.sidebar.number_input(
    "Underlying Asset Price", min_value=0.0, max_value=1000000.0, value=30.00, step=0.01, format="%.2f")
strike_price = st.sidebar.number_input(
    "Strike Price", min_value=0.0, max_value=1000000.0, value=50.00, step=0.01, format="%.2f")
time_to_expiry = st.sidebar.number_input(
    "Time to Expiry (years)", min_value=0.0, max_value=5.0, value=250/365.0, step=0.01, format="%.2f",
    help="Enter the time to expiry in years (e.g., 0.68 for 250 days).")
volatility_percent = st.sidebar.number_input(
    "Volatility (%)", min_value=0.0, max_value=300.0, value=30.0, step=0.1, format="%.1f",
    help="Enter the volatility as a percentage (e.g., 30 for 30%).")
dividend_yield_percent = st.sidebar.number_input(
    "Dividend Yield (%)", min_value=0.0, max_value=15.0, value=0.0, step=0.1, format="%.1f",
    help="Enter the dividend yield as a percentage (e.g., 2 for 2%). Set to 0 for non-dividend-paying stocks.")

# Add a Calculate button in the sidebar
calculate_button = st.sidebar.button("Calculate")

# Only perform calculations and display results if the button is clicked
if calculate_button:
    # Validate inputs: ensure all inputs except dividend yield are non-zero
    if risk_free_rate_percent <= 0.0:
        st.error("Risk-Free Rate (%) must be greater than 0.")
    elif underlying_price <= 0.0:
        st.error("Underlying Asset Price must be greater than 0.")
    elif strike_price <= 0.0:
        st.error("Strike Price must be greater than 0.")
    elif time_to_expiry <= 0.0:
        st.error("Time to Expiry (years) must be greater than 0.")
    elif volatility_percent <= 0.0:
        st.error("Volatility (%) must be greater than 0.")
    else:
        # Convert percentage inputs to decimals for the Black-Scholes calculation
        risk_free_rate = risk_free_rate_percent / 100.0
        volatility = volatility_percent / 100.0
        dividend_yield = dividend_yield_percent / 100.0

        # Calculate both Call and Put option prices and Greeks
        call_result = black_scholes(underlying_price, strike_price,
                                    time_to_expiry, risk_free_rate, volatility, dividend_yield, option_type="Call")
        put_result = black_scholes(underlying_price, strike_price,
                                   time_to_expiry, risk_free_rate, volatility, dividend_yield, option_type="Put")

        # Display results
        st.header("Option Prices and Greeks")
        col1, col2 = st.columns(2)

        with col1:
            st.subheader("Call Price")
            st.write(f"{call_result['price']:.2f}")

        with col2:
            st.subheader("Put Price")
            st.write(f"{put_result['price']:.2f}")

        # Display Greeks (using Call option Greeks as default)
        st.subheader("")
        col1, col2, col3, col4, col5 = st.columns(5)
        col1.metric("Delta", f"{call_result['delta']:.2f}")
        col2.metric("Gamma", f"{call_result['gamma']:.2f}")
        col3.metric("Theta", f"{call_result['theta']:.2f}")
        col4.metric("Vega", f"{call_result['vega']:.2f}")
        col5.metric("Rho", f"{call_result['rho']:.2f}")