
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

#include "ValueKernel.h"

registerMooseObject("raccoonApp",ValueKernel);

template <>
InputParameters
validParams<ValueKernel>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<Real>("coeff", 1.0, "coefficient");
  return params;
}

ValueKernel::ValueKernel(const InputParameters & parameters)
  : Kernel(parameters),
  _coeff(getParam<Real>("coeff"))
{
}

Real ValueKernel::computeQpResidual()
{


    return _coeff*_u[_qp]*_test[_i][_qp];
}

Real ValueKernel::computeQpJacobian()
{

    return _coeff*_phi[_j][_qp]*_test[_i][_qp];

}
