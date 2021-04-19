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

#ifndef ValueKernelCoupled_H
#define ValueKernelCoupled_H

#include "Kernel.h"

class ValueKernelCoupled;

template<>
InputParameters validParams<ValueKernelCoupled>();


class ValueKernelCoupled : public Kernel
{
public:
  ValueKernelCoupled(const InputParameters & parameters);

private:

  unsigned int _c_var;
  const VariableValue & _coupled;
  const Real & _coeff;
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
};


#endif /* ValueKernelCoupled_H */
