
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
