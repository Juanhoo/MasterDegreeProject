/** @file */ 

#ifndef IMP_LUKASIEWICZ_H
#define IMP_LUKASIEWICZ_H

#include <iostream>
#include "implication.h"

namespace ksi
{
   struct imp_lukasiewicz : virtual public implication
   {
      /** @return value of fuzzy implication: \f$x \rightarrow y = \min[1, 1 - x + y]\f$*/
      virtual double imply (double, double) const;
      /** @return G function for ANNBFIS NFS  */
      virtual double G (double F, double w) const;
      /** @return differential dG / dF for  ANNBFIS NFS */
      virtual double dG_dF (double F, double w) const;
      /** @return differential dG / dw for  ANNBFIS NFS */
      virtual double dG_dw (double F, double w) const;
      /** cloning method */
      virtual implication * clone() const; 
      
      virtual ~imp_lukasiewicz();
      
      virtual std::ostream & Print (std::ostream & ss) const;
   };
}

#endif
