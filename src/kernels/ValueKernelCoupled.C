//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

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
