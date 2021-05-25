//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADValueKernelCoupled.h"

registerMooseObject("raccoonApp",ADValueKernelCoupled);

template <>
InputParameters
validParams<ADValueKernelCoupled>()
{
  InputParameters params = validParams<ADKernel>();
  params.addRequiredCoupledVar("coupled", "Coupled variableacting in the kernel");
  params.addParam<Real>("coeff", 1.0, "coefficient");
  return params;
}

ADValueKernelCoupled::ADValueKernelCoupled(const InputParameters & parameters)
  : ADKernel(parameters),
  _c_var(coupled("coupled")),
  _coupled(adCoupledValue("coupled")),
  _coeff(getParam<Real>("coeff"))
{
}

ADReal ADValueKernelCoupled::computeQpResidual()
{
  return _coeff * _coupled[_qp] * _test[_i][_qp];
}
