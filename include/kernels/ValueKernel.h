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

#ifndef ValueKernel_H
#define ValueKernel_H

#include "Kernel.h"

class ValueKernel;

template<>
InputParameters validParams<ValueKernel>();


class ValueKernel : public Kernel
{
public:
  ValueKernel(const InputParameters & parameters);
private:
  const Real & _coeff;
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
};


#endif /* ValueKernel_H */
