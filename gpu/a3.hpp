/*  Sai Vishwanath  
 *  Venkatesh 
 *  saivishw
 */
#include <cuda_runtime_api.h>
#define _USE_MATH_DEFINES 
#include <cmath>
#ifndef A3_HPP
#define A3_HPP

__global__
void gaussian_in_device(int n, int m, float h, const float* x, float* y){
  extern __shared__ float fastbuf[];
 
  float* x_s =  fastbuf;

  int i = threadIdx.x + blockIdx.x * m;

  float sum = 0.0;

  int xh = x[i];
  __syncthreads();

  for(int k=0;k<n/m;++k){   
    x_s[threadIdx.x] = x[threadIdx.x+(k*m)];
    __syncthreads();
    for(int l=0;l<m;++l){
      if ((l+k*m)<n){
        sum+= ((((expf(-(pow(xh,2.0)/2.0))/sqrt(2.0*M_PI)))/h)*(xh-x_s[l]))/h;
      }
      else{
        break;
      }
    }
  }
  y[i] = sum/(n*h);
}  
  

void gaussian_kde(int n, float h, const std::vector<float>& x, std::vector<float>& y) {

    // // DEBUG CODE
    // for(int i=0;i<x.size();i++){
    //   std::cout<<"\nX"<<i<<" :"<<x[i];
    // }
    // //DEBUG END
    float* d_x;
    float* d_y;
    
    int size  = n * sizeof(float);

    cudaMalloc(&d_x, size);
    cudaMalloc(&d_y, size);

    cudaMemcpy(d_x, x.data(), size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y.data(), size, cudaMemcpyHostToDevice);

    // Call host function that calls 

    const int block_size = 256;
    // int m = block_size;
    // int num_blocks = (n + block_size - 1 ) / block_size;
    // gaussian_in_device<<<num_blocks, block_size, block_size>>>(n, m, h, d_x, d_y);
    gaussian_in_device<<<(n+ block_size -1)/block_size, block_size, block_size*sizeof(float)>>>(n,block_size, h, d_x, d_y);

    if (cudaMemcpy(y.data(), d_y, size, cudaMemcpyDeviceToHost) !=cudaSuccess){
      std::cout<< "FAILED MEMCOPY ";
    }
    cudaFree(d_x);

    // // DEBUG CODE
    // for(int i=0;i<y.size();i++){
    //   std::cout<<"\nX"<<i<<" :"<<y[i];
    // }
    // //DEBUG END
} // gaussian_kde

#endif // A3_HPP


