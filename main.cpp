#include <iostream>
#include <math.h>
using namespace std;

const double RSQRT2PI = 0.398942280401432677939946;

//definition of menuchoice function
int MenuChoice()
{
    cout << "Please choose one of the following options:" << endl;
    cout << "1. Evaluate call option price." << endl;
    cout << "2. Evaluate put option price." << endl;
    cout << "3. Evaluate Delta Greek." << endl;
    cout << "4. Evaluate Gamma Greek." << endl;
    cout << "5. Evaluate Vega Greek." << endl;
    
    int MenuChoice;
    cin >> MenuChoice;
    
    if (MenuChoice < 1 || MenuChoice > 5)
    {
        cout << "You have entered a wrong option" << endl;
        return 0;
    }
    return MenuChoice;
}


//definition of read data function
void ReadData(long& Notional, double& StockPrice, double& StrikePrice,
           double& YearsToExpiry, double& StockVolatility,
           double& InterestRate, double& DividendYield)
{
    cout << "Please enter a number of stocks: ";
    cin >> Notional;
    
    cout << "Please enter a stock price: ";
    cin >> StockPrice;
    
    cout << "Please enter stock volatility (in percents): ";
    cin >> StockVolatility;
    StockVolatility /= 100;
    
    cout << "Please enter a strike price: ";
    cin >> StrikePrice;
    
    cout << "Please enter time to maturity: ";
    cin >> YearsToExpiry;    
    
    cout << "Please enter an interest rate (in percents): ";
    cin >> InterestRate;
    InterestRate /= 100;
    
    cout << "Please enter a dividend rate (in percents): ";
    cin >> DividendYield;
    DividendYield /= 100;
}


// ProbabilityNormalDistribution function definition which calculates and return (I DONT KNOW WHAT)
double ProbabilityNormalDistribution(double d)
{
    return exp(-d * d / 2) * RSQRT2PI;
}


//CumulativeNormalDistribution function definition which takes input dplus and dminus 
//from calloptionpricing func and retuns cnd of these terms 
double CumulativeNormalDistribution(double d)
{
    const double A1 = 0.31938153;
    const double A2 = -0.356563782;
    const double A3 = 1.781477937;
    const double A4 = -1.821255978;
    const double A5 = 1.330274429;
    const double K = 1.0 / (1.0 + 0.2316419 * fabs(d));
    double cnd = RSQRT2PI * exp(-0.5 * d * d) * (K * (A1 + K * (A2 + K * (A3 + K * (A4 + K * A5)))));
    if (d > 0)
        cnd = 1.0 - cnd;
    return cnd;
}


// call_option_price function definition takes user input and calculates, return option price
double CallOptionPrice(long Notional, double StockPrice, double StrikePrice,
                       double StockVolatility, double YearsToExpiry, double InterestRate,
                       double DividendYield)
{
    //calculating dplus term
    double DPlus = (log(StockPrice / StrikePrice) + (InterestRate - DividendYield + StockVolatility * StockVolatility / 2) * YearsToExpiry) / (StockVolatility * sqrt(YearsToExpiry));
    
    //calculating dminus term
    double DMinus = DPlus - StockVolatility * sqrt(YearsToExpiry);
    
    //calculating option price with help of dplus and dminus term
    double OptionPrice = Notional * (StockPrice * exp(-DividendYield * YearsToExpiry) * CumulativeNormalDistribution(DPlus) - StrikePrice * exp(-InterestRate * YearsToExpiry) * CumulativeNormalDistribution(DMinus));

    return OptionPrice;
}


// put_option_price function definition takes user input and calculates, return option price
double PutOptionPrice(long Notional, double StockPrice, double StrikePrice,
                      double StockVolatility, double YearsToExpiry,
                      double InterestRate, double DividendYield)
{
    double DPlus = (log(StockPrice / StrikePrice) + (InterestRate - DividendYield + StockVolatility * StockVolatility / 2) * YearsToExpiry) / (StockVolatility * sqrt(YearsToExpiry));
    double DMinus = DPlus - StockVolatility * sqrt(YearsToExpiry);
    double OptionPrice = Notional * (-StockPrice * exp(-DividendYield * YearsToExpiry) * CumulativeNormalDistribution(-DPlus) + StrikePrice * exp(-InterestRate * YearsToExpiry) * CumulativeNormalDistribution(-DMinus));
    return OptionPrice;
}


// deltagreek value function definition takes user input and calculates, return greek value
double DeltaGreekValue(long Notional, double StockPrice, double StrikePrice,
                       double StockVolatility, double YearsToExpiry, double InterestRate,
                       double DividendYield)
{
    double DPlus = (log(StockPrice / StrikePrice) + (InterestRate - DividendYield + StockVolatility * StockVolatility / 2) * YearsToExpiry) / (StockVolatility * sqrt(YearsToExpiry));
    return Notional * exp(-DividendYield * YearsToExpiry) * CumulativeNormalDistribution(DPlus);
}


// deltagamma value function definition takes user input and calculates, return gamma value
double GammaGreekValue(long Notional, double StockPrice,
                       double StrikePrice, double StockVolatility, double YearsToExpiry,
                       double InterestRate, double DividendYield)
{
    double DPlus = (log(StockPrice / StrikePrice) + (InterestRate - DividendYield + StockVolatility * StockVolatility / 2) * YearsToExpiry) / (StockVolatility * sqrt(YearsToExpiry));
    return Notional * exp(-DividendYield * YearsToExpiry) * ProbabilityNormalDistribution(DPlus) / (StockVolatility * StockPrice * sqrt(YearsToExpiry));
}


// deltavega value function definition takes user input and calculates, return vega value
double VegaGreekValue(long Notional, double StockPrice, double StrikePrice,
                      double StockVolatility, double YearsToExpiry, double InterestRate,
                      double DividendYield)
{
    double DPlus = (log(StockPrice / StrikePrice) + (InterestRate - DividendYield + StockVolatility * StockVolatility / 2) * YearsToExpiry) / (StockVolatility * sqrt(YearsToExpiry));
    return Notional * exp(-DividendYield * YearsToExpiry) * StockPrice * ProbabilityNormalDistribution(DPlus) * sqrt(YearsToExpiry);
}



int main()
{

    // declaring variable to be taken input from user
    long Notional;
    double StockPrice;
    double StockVolatility;
    double StrikePrice;
    double YearsToExpiry;
    double InterestRate;
    double DividendYield;

    // calling menu function
    int menu = MenuChoice();

    //parity check again for input is provided in given range or not 
    if (menu > 0 && menu < 6)
    {
        // calling read_data function to take input values for the corresponding operations choosen by user
        ReadData(Notional, StockPrice,
                 StrikePrice, YearsToExpiry, StockVolatility,
                 InterestRate, DividendYield);
    }

 // displaying cases as per choosing menu option
    switch (menu)
    {
    case 1:
        cout << "Call option price is: " << CallOptionPrice(Notional, StockPrice, StrikePrice, StockVolatility, YearsToExpiry, InterestRate, DividendYield) << endl;
        break;
    case 2:
        cout << "Put option price is: " << PutOptionPrice(Notional, StockPrice, StrikePrice, StockVolatility, YearsToExpiry, InterestRate, DividendYield) << endl;
        break;
    case 3:
        cout << "Delta Greek value is: " << DeltaGreekValue(Notional, StockPrice, StrikePrice, StockVolatility, YearsToExpiry, InterestRate, DividendYield) << endl;
        break;
    case 4:
        cout << "Gamma Greek value is: " << GammaGreekValue(Notional, StockPrice, StrikePrice, StockVolatility, YearsToExpiry, InterestRate, DividendYield) << endl;
        break;
    case 5:
        cout << "Vega Greek value is: " << VegaGreekValue(Notional, StockPrice, StrikePrice, StockVolatility, YearsToExpiry, InterestRate, DividendYield) << endl;
        break;
    default:
        break;
    }


  return 0;
}