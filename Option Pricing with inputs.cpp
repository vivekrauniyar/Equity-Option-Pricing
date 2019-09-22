#include <iostream>
#include <cmath>

int main(int argc, char **argv)
{
	double S,K,r,v,t;
	
	std::cout << "Enter Spot Price: \n";
	std::cin >> S;
	
	std::cout << "\nEnter Strike of Option: \n";
	std::cin >> K;
	
	std::cout << "\nEnter Interest Rate: \n";
	std::cin >> r;
	
	std::cout << "\nEnter Volatility of Underlying: \n";
	std::cin >> v;
	
	std::cout << "\nEnter Option Expiry: \n";
	std::cin >> t;
	
	std::cout << "Underlying:      " << S << std::endl;
    std::cout << "Strike:          " << K << std::endl;
    std::cout << "Risk-Free Rate:  " << r << std::endl;
    std::cout << "Volatility:      " << v << std::endl;
    std::cout << "Maturity:        " << t << std::endl;
	
	return 0;
}

