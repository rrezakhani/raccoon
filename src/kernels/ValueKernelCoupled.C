
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

#include "ValueKernelCoupled.h"

registerMooseObject("raccoonApp",ValueKernelCoupled);

template <>
InputParameters
validParams<ValueKernelCoupled>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("coupled", "Coupled variableacting in the kernel");
  params.addParam<Real>("coeff", 1.0, "coefficient");
  return params;
}

ValueKernelCoupled::ValueKernelCoupled(const InputParameters & parameters)
  : Kernel(parameters),
  _c_var(coupled("coupled")),
  _coupled(coupledValue("coupled")),
  _coeff(getParam<Real>("coeff"))
{
}

Real ValueKernelCoupled::computeQpResidual()
{


    return _coeff * _coupled[_qp] * _test[_i][_qp];
}

Real ValueKernelCoupled::computeQpJacobian()
{

    return 0.0;

}



Real ValueKernelCoupled::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _c_var)
  {
    return _coeff * _phi[_j][_qp] * _test[_i][_qp];
  }

  return 0.0;
}
