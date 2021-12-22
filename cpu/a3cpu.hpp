#define _USE_MATH_DEFINES 
#include <cmath>
#ifndef A3_HPP
#define A3_HPP

void gaussian_kde(int n, float h, const std::vector<float>& x, std::vector<float>& y) {

    // // DEBUG CODE
    // for(int i=0;i<x.size();i++){
    //   std::cout<<"\nX"<<i<<" :"<<x[i];
    // }
    // //DEBUG END

    float K;
    float sum;
    // Call host function that calls 
    for(int i=0;i<x.size();i++){
      K = ((exp(-(pow(x[i],2.0)/2.0))/sqrt(2*M_PI)))/h;
      sum = 0.0;
      for(int j=0;j<x.size();j++){
        sum+= (K*(x[i]-x[j]))/h;
      }
      y[i] = sum/(n*h);
    }

    // // DEBUG CODE
    // for(int i=0;i<y.size();i++){
    //   std::cout<<"\nX"<<i<<" :"<<y[i];
    // }
    // //DEBUG END
} // gaussian_kde

#endif // A3_HPP
