#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <iomanip>

//Standard normal probability density function

double norm_pdf(const double& x) {
    return (1.0/(pow(2*M_PI,0.5)))*exp(-0.5*x*x);
}

// An approximation to the cumulative distribution function
// for the standard normal distribution
// Note: This is a recursive function

double norm_cdf(const double& x) {
    double k = 1.0/(1.0 + 0.2316419*x);
    double k_sum = k*(0.319381530 + k*(-0.356563782 + k*(1.781477937 + k*(-1.821255978 + 1.330274429*k))));

    if (x >= 0.0) {
        return (1.0 - (1.0/(pow(2*M_PI,0.5)))*exp(-0.5*x*x) * k_sum);
    } else {
        return 1.0 - norm_cdf(-x);
    }
}

// This calculates d_j, for j in {1,2}. This term appears in the closed
// form solution for the European call or put price

double d_j(const int& j, const double& S, const double& K, const double& r, const double& v, const double& t) {
    return (log(S/K) + (r + (pow(-1,j-1))*0.5*v*v)*t)/(v*(pow(t,0.5)));
}


double Call_Opt_Pricer (const double& S,const double& K,const double& r,const double& v,const double& t)
{
	double Call_Opt_Premium;
	
	Call_Opt_Premium = S*norm_cdf(d_j(1,S,K,r,v,t)) - K*exp(-r*t)*norm_cdf(d_j(2,S,K,r,v,t));
	
	return Call_Opt_Premium;
}

double Put_Opt_Pricer (const double& S,const double& K,const double& r,const double& v,const double& t)
{
	double Put_Opt_Premium;
	
	Put_Opt_Premium = K*exp(-r*t)*norm_cdf(-d_j(2,S,K,r,v,t)) - S*norm_cdf(-d_j(1,S,K,r,v,t));
	
	return Put_Opt_Premium;
}

double Greeks (const double& S,const double& K,const double& r,const double& v,const double& t, const char& Opt_type)
{
	double New_Opt_Premium;
	
	if(Opt_type=='P')
			{
				New_Opt_Premium = Put_Opt_Pricer(S,K,r,v,t) ;
				
			}else if(Opt_type=='C'){ 
				
				New_Opt_Premium = Call_Opt_Pricer(S,K,r,v,t);
				
			}	
	return New_Opt_Premium; 		
}

double * Vol_Smile_Fitting ()
{

			double Vol[20][20],SABR_param[10],Vol_hardcoded[20]={0.24,0.20,0.17,0.15,0.13,0.115,0.125,0.135,0.15,0.18,0.21};
			double ATM_Vol,Volatility;
			int * loc;
			double ATM_Strike,Strike;
			double Strike_Increment,Strike_Range,Strike_Range_Low,Strike_Range_High,Strike_count;
			double A,B,C;
			double A_high=10,B_high=10,C_high=10;
			double A_low=-10.0,B_low=-10.0,C_low=-10.0;
			double A_Fitted,B_Fitted,C_Fitted;
			int ATM;
			//std::cout << "Enter ATM Vol\n";
			//std::cin >> ATM_Vol;
			
			ATM_Strike=100, Strike_Increment=5, Strike_Range_Low=75,Strike_Range_High=125;
			/*			
			std::cout << "Enter ATM Strike\n";
			std::cin >> ATM_Strike;
			
			std::cout << "Enter Strike Increment for the Smile Grid. Please enter multiples of 5.\n";
			std::cin >> Strike_Increment;
			
			std::cout << "Enter Lowest Strike for the Strike Range for the Smile Grid. Please enter multiples of 5.\n";
			std::cin >> Strike_Range_Low;
			
			std::cout << "Enter Highest Strike for the Strike Range for the Smile Grid. Please enter multiples of 5.\n";
			std::cin >> Strike_Range_High;
			
			*/
			Strike_count = (Strike_Range_High - Strike_Range_Low)/Strike_Increment+1;
			
			Strike = Strike_Range_Low;
			
			std::cout << Strike_count << "      " << Strike << std::endl;
			
			for(int i=0;i<Strike_count;i++){
			
			Vol[0][i]=Strike/ATM_Strike;		
			std::cout << "\n";
			
			//std::cout << "Enter Implied Vol in % for the Strike  : " << Strike << std::endl;
			//std::cin >> Volatility;
			//Vol[1][i]=Volatility/100;
			
			Vol[1][i]=Vol_hardcoded[i];
			
			if(Strike==ATM_Strike){
				ATM=i;
				//ATM_Vol=Volatility/100;
				ATM_Vol=Vol_hardcoded[i];
			}
			Strike = Strike + Strike_Increment;
			}
			
			std::cout << "Strike      Implied Vol " << std::endl;
			
			for (int i=0;i<Strike_count;i++){
			std::cout <<"  " << Vol[0][i] << "             " << Vol[1][i] << std::endl;
			}
			
	/////////////////////////////////////////////////////////////////////////////////////////////////		
	//  Vol Smile Fitting using quadratic equation		
	////////////////////////////////////////////////////////////////////////////////////////////////
			
			double Min_Std_err=1, Std_err=0,Err=0, iter=0, ATM_Diff=0.2,ATM_Lowest_Diff=0.5,A_ATM_lowest=0,B_ATM_lowest=0,C_ATM_lowest=0;
			double Min_Std_err_ATM_Fixed=1,A_Fitted_ATM_Fixed,B_Fitted_ATM_Fixed,C_Fitted_ATM_Fixed;
			
			for(A=A_low;A<=A_high;A+=0.05){
				for(B=B_low;B<=B_high;B+=0.05){
					for(C=C_low;C<=C_high;C+=0.05){
						Std_err=0;
						Err=0;
						for(int i=0;i<Strike_count;i++){
									
									if(Vol[0][i]!=1){
									Err=pow((Vol[1][i]-(A*pow(Vol[0][i],2.0)+B*Vol[0][i]+C)),2.0)*(1/sqrt(abs(ATM_Strike*Vol[0][i]-ATM_Strike)));
									}
									if(Vol[0][i]==1){
									Err=pow((Vol[1][i]-(A*pow(Vol[0][i],2.0)+B*Vol[0][i]+C)),2.0);
									}
									Std_err=Std_err+Err;	
						}
						iter++;
						
						Std_err= pow(Std_err,0.5)/Strike_count;
						
						//std::cout << ".\n" << iter << "  " << A << "   " << B << "     " << C << "     "<< Strike_count << "    " << Std_err << std::endl;
						
						ATM_Diff=abs(A*pow(Vol[0][ATM],2.0)+B*Vol[0][ATM]+C-ATM_Vol);
						
						if(ATM_Lowest_Diff>ATM_Diff){
							ATM_Lowest_Diff=ATM_Diff;
							A_ATM_lowest=A;
							B_ATM_lowest=B;
							C_ATM_lowest=C;
						for(int i=0;i<Strike_count;i++){
							Vol[3][i]=A*pow(Vol[0][i],2.0)+B*Vol[0][i]+C;
							}
						}
						
						if(Min_Std_err>Std_err){
							Min_Std_err=Std_err;
							A_Fitted = A;
							B_Fitted = B;
							C_Fitted = C;
						}
					}
				}
			}
			
			for(int i=0;i<Strike_count;i++){
			Vol[2][i]=A_Fitted*pow(Vol[0][i],2.0)+B_Fitted*Vol[0][i]+C_Fitted;		
			}
			
	/////////////////////////////////////////////////////////////////////////////////////////////////		
	//  Vol Smile Fitting using ATM Volatility for constant term in quadratic equation		
	////////////////////////////////////////////////////////////////////////////////////////////////
			
			for(A=A_low+5;A<=A_high-5;A+=0.05){
				for(B=B_low+5;B<=B_high-5;B+=0.05){
						Std_err=0;
						Err=0;
						for(int i=0;i<Strike_count;i++){
									if(Vol[0][i]!=1.0){
									Err=pow((Vol[1][i]-(A*pow(Vol[0][i],2.0)+B*Vol[0][i]+ATM_Vol)),2.0)*(1/sqrt(abs(ATM_Strike*Vol[0][i]-ATM_Strike)));
									}
									/*if(Vol[0][i]==1.0){
									Err=pow((Vol[1][i]-(A*pow(Vol[0][i],2.0)+B*Vol[0][i]+ATM_Vol)),2.0);
									}*/
									Std_err=Std_err+Err;	
						}
												
						Std_err= pow(Std_err,0.5)/Strike_count;
	
						//std::cout << ".\n" << "  " << A << "   " << B << "     " << ATM_Vol << "     "<< Strike_count << "    " << Std_err << std::endl;
															
						if(Min_Std_err_ATM_Fixed>Std_err){
							Min_Std_err_ATM_Fixed=Std_err;
							A_Fitted_ATM_Fixed = A;
							B_Fitted_ATM_Fixed = B;
							C_Fitted_ATM_Fixed = ATM_Vol;
						}
					}
				}
			
			
			for(int i=0;i<Strike_count;i++){
				if(Vol[0][i]!=1.0){
					Vol[4][i]=A_Fitted_ATM_Fixed*pow(Vol[0][i],2.0)+B_Fitted_ATM_Fixed*Vol[0][i]+C_Fitted_ATM_Fixed;
				}else {
					Vol[4][i]=C_Fitted_ATM_Fixed;
				}
			//std::cout << Vol[4][i];	
			}
			
			
			SABR_param[0]=A_Fitted;
			SABR_param[1]=B_Fitted;
			SABR_param[2]=C_Fitted;
			SABR_param[3]=A_Fitted_ATM_Fixed;
			SABR_param[4]=B_Fitted_ATM_Fixed;
			SABR_param[5]=C_Fitted_ATM_Fixed;		
			                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
			
			std::cout << "\n\n********************************************************\n";
			std::cout << "The best fit estimates for the parameters are the below\n";
			std::cout << "********************************************************\n";
			std::cout << "A   :" << A_Fitted << std::endl;
			std::cout << "B   :" << B_Fitted << std::endl;
			std::cout << "C   :" << C_Fitted << std::endl;
			std::cout << "Minimum Standard error   :" << Min_Std_err << std::endl;
			std::cout << "ATM Lowest Difference   :" << ATM_Lowest_Diff << std::endl;
			
			std::cout << "A_ATM_Fixed  :" << A_Fitted_ATM_Fixed << std::endl;
			std::cout << "B_ATM_Fixed  :" << B_Fitted_ATM_Fixed << std::endl;
			std::cout << "C_ATM_Fixed  :" << C_Fitted_ATM_Fixed << std::endl;
			std::cout << "Minimum Standard error ATM Fixed  :" << Min_Std_err_ATM_Fixed << std::endl;
			
			std::cout << "\n*******************************************************************************************************************\n";
			std::cout << "\nRelative Strike        Implied Vol           Fitted_Vol            ATM_Fixed_Fitted_Vol          ATM_Lowest_Diff_Vols" << std::endl;
			std::cout << "*********************************************************************************************************************\n";
			
			for (int i=0;i<Strike_count;i++){
						std::cout << "     "<< std::setprecision(6) << Vol[0][i] << "                "<< std::setprecision(6)<< Vol[1][i] << "                "<< std::setprecision(6) << Vol[2][i] << "                       "<< std::setprecision(6) << Vol[4][i]<< "                        "<< std::setprecision(6) << Vol[3][i] << std::endl;
			}
			
			
				
			return SABR_param;
			
}


int main(int argc, char **argv)
{
	double S,K,r,v,t;
	char Opt_type;
	double *pVol;
	int Vol_Smile_flg=0;
	double Opt_Premium, Cal_Opt_Premium, Cal_Opt_Premium_ATM_Fixed,Opt_Premium_tweak, Cal_Vol,Cal_Vol_ATM_Fixed;
	double S_rel_positivetweak, S_rel_negativetweak,S_abs_positivetweak,S_abs_negativetweak;
	double delta,gamma,vega,theta,rho;
	double delta1,gamma1,vega1,theta1,rho1;
	
	//Vol Grid Declaration Vol[Strike][Volatility]
	double Vol[50][50];
	char Job_select;
	
	std::cout << "What you would like to generate: Enter P for Option Pricing / Enter V for Vol Smile Fitting \n";
	std::cin >> Job_select;
	
	if(Job_select == 'V')
	{
		pVol=Vol_Smile_Fitting();
		Vol_Smile_flg=1;
	}

	std::cout<<"\n Enter Option Pricing Parameters\n";
	
	std::cout << "Enter Spot Price: \n";
	std::cin >> S;
	
	std::cout << "Enter Strike of Option: \n";
	std::cin >> K;
	
	std::cout << "Enter Interest Rate (in fraction): \n";
	std::cin >> r;
	
	std::cout << "Enter Volatility of Underlying (in fraction): \n";
	std::cin >> v;
	
	std::cout << "Enter Option Expiry (in years): \n";
	std::cin >> t;
	
	std::cout << "Which Option do you want to price: Enter C for Call/ P for Put \n";
	std::cin >> Opt_type;
	
		if(Vol_Smile_flg==1){
		Cal_Vol=*(pVol+0)*pow(K/S,2)+*(pVol+1)*K/S+*(pVol+2);
		Cal_Vol_ATM_Fixed=*(pVol+3)*pow(K/S,2)+*(pVol+4)*K/S+*(pVol+5);
	    }
			if(Opt_type=='P'){
			    Opt_Premium = Put_Opt_Pricer(S,K,r,v,t) ;
			    if(Vol_Smile_flg==1){
				Cal_Opt_Premium=Put_Opt_Pricer(S,K,r,Cal_Vol,t) ;
			    Cal_Opt_Premium_ATM_Fixed=Put_Opt_Pricer(S,K,r,Cal_Vol_ATM_Fixed,t);
				}	
			}else if(Opt_type=='C'){ 
				Opt_Premium = Call_Opt_Pricer(S,K,r,v,t);
				if(Vol_Smile_flg==1){
				Cal_Opt_Premium=Call_Opt_Pricer(S,K,r,Cal_Vol,t) ;
			    Cal_Opt_Premium_ATM_Fixed=Call_Opt_Pricer(S,K,r,Cal_Vol_ATM_Fixed,t);
				}
			}else{
				std::cout << "Invalid Option Type \n";
				std::cout << "Closing Program \n";
				return 0; 
			}

	/*
	Calculate Option Greeks using two different tweak methodology
	*/
 
	// Greeks Calculation Using Relative Tweaks
 	delta=Greeks(S*1.01,K,r,v,t,Opt_type)-Opt_Premium;
 	S_rel_positivetweak=S*1.01;
 	S_rel_negativetweak=S*0.99;
 	gamma=(Greeks(S_rel_positivetweak,K,r,v,t,Opt_type)-Greeks(S_rel_negativetweak,K,r,v,t,Opt_type))/(S_rel_positivetweak-S_rel_negativetweak);
	rho=Greeks(S,K,r*1.01,v,t,Opt_type)-Opt_Premium;
	vega=Greeks(S,K,r,v*1.01,t,Opt_type)-Opt_Premium;
	theta=Greeks(S,K,r,v,t*(1-0.01),Opt_type)-Opt_Premium; 

	// Greeks Calculation Using Absolute Tweaks	
	delta1=Greeks(S+1,K,r,v,t,Opt_type)-Opt_Premium;
 	S_abs_positivetweak=S+1;
 	S_abs_negativetweak=S-1;
	gamma1=(Greeks(S_abs_positivetweak,K,r,v,t,Opt_type)-Greeks(S_abs_negativetweak,K,r,v,t,Opt_type))/(S_abs_positivetweak-S_abs_negativetweak);
	rho1=Greeks(S,K,r+0.01,v,t,Opt_type)-Opt_Premium;
	vega1=Greeks(S,K,r,v+.01,t,Opt_type)-Opt_Premium;
	theta1=Greeks(S,K,r,v,t-(1/365),Opt_type)-Opt_Premium; 
	
	std::cout << "Underlying:         " << S << std::endl;
    std::cout << "Strike:             " << K << std::endl;
    std::cout << "Risk-Free Rate:     " << r << std::endl;
    std::cout << "Volatility:         " << v << std::endl;
    std::cout << "Maturity:           " << t << std::endl; 
	std::cout << "Option Type:        " << Opt_type << std::endl;
	std::cout << "Option Premium:     " << Opt_Premium << std::endl;
	if(Vol_Smile_flg==1){
		std::cout << "Calibrated Option Premium:       "<< Cal_Opt_Premium << std::endl;
		std::cout << "Calibrated Option Volatility:       "<< Cal_Vol << std::endl;
		std::cout << "Calibrated Option Premium ATM Fixed:       "<< Cal_Opt_Premium_ATM_Fixed<< std::endl;
		std::cout << "Calibrated Option Volatility ATM Fixed:       "<< Cal_Vol_ATM_Fixed<< std::endl;
		}	
	std::cout << "\nOption Greeks are as below\n" << std::endl;
	std::cout << "\n******************************" << std::endl;
	std::cout << "Relative Greeks:         " << std::endl;
	std::cout << "******************************\n" << std::endl;
	std::cout << "Delta:              " << delta << std::endl;
    std::cout << "Gamma:              " << gamma << std::endl;
    std::cout << "Vega:               " << vega << std::endl;
    std::cout << "Rho:                " << rho << std::endl;
    std::cout << "Theta:              " << theta << std::endl;
	std::cout << "\n******************************" << std::endl;	
	std::cout << "Absolute Greeks:         " << std::endl;
	std::cout << "******************************\n" << std::endl;
	std::cout << "Delta:              " << delta1 << std::endl;
    std::cout << "Gamma:              " << gamma1 << std::endl;
    std::cout << "Vega:               " << vega1 << std::endl;
    std::cout << "Rho:                " << rho1 << std::endl;
    std::cout << "Theta:              " << theta1 << std::endl;
	
	return 0;
}

