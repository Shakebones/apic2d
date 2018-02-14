#include "MathDefs.h"

#ifndef _KERNEL_H_
#define _KERNEL_H_

namespace kernel {
  
  // Equ. (2) in [Akinci et al. 2013]
  inline scalar akinci_cohesion_kernel( const scalar& r, const scalar& h ) {
    scalar coeff = 32. / M_PI / pow(h, 9.);
    
    if(2. * r > h && r <= h)
    {
      return coeff * (h - r) * (h - r) * (h - r) * r * r * r;
    } else if(r > 0 && 2. * r <= h)
    {
      return coeff * (2. * (h - r) * (h - r) * (h - r) * r * r * r - pow(h, 6.) / 64.);
    } else {
      return 0.0;
    }
  }
  
  inline scalar smooth_kernel( const scalar& r2, const scalar& h ) {
    return std::max( pow(1.0 - r2 / (h * h), 3.0), 0.0 );
  }
  
  inline scalar poly6_kernel( const scalar& r2, const scalar& h ) {
    if(r2 <= h*h) return 4.0 / (M_PI * pow(h, 8.0)) * pow(h*h - r2, 3.0);
    else return 0.0;
  }
  
  inline Vector2s grad_poly6_kernel( const Vector2s& d, const scalar& h ) {
    scalar r2 = d.squaredNorm();
    if(r2 <= h*h) return -24.0 / (M_PI * pow(h, 8.0)) * pow(h*h - r2, 2.0) * d;
    else return Vector2s::Zero();
  }
  
  inline scalar smooth_kernel_laplacian( const scalar& r2, const scalar& h ) {
    scalar x2 = sqrt( r2 / (h*h));
    return x2 > 1.0 ? 0.0 : (1.0 - x2);
  }
  
  inline scalar sharp_kernel( const scalar& r2, const scalar& h ) {
    return std::max( h * h / std::max(r2, 1.0e-5) - 1.0, 0.0 );
  }
  
  inline scalar linear_kernel(const Vector2s& d, const scalar& h) {
    return std::max( (1.0 - fabs(d(0) / h)) * (1.0 - fabs(d(1) / h)), 0.0 );
  }
}

#endif