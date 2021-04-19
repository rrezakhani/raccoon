/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef ADValueKernelCoupled_H
#define ADValueKernelCoupled_H

#include "ADKernel.h"

class ADValueKernelCoupled;

template<>
InputParameters validParams<ADValueKernelCoupled>();


class ADValueKernelCoupled : public ADKernel
{
public:
  ADValueKernelCoupled(const InputParameters & parameters);

private:

  unsigned int _c_var;
  const ADVariableValue & _coupled;
  const Real & _coeff;
protected:
  virtual ADReal computeQpResidual();
};


#endif /* ValueKernelCoupled_H */
