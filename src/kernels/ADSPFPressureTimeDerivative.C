//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADSPFPressureTimeDerivative.h"

registerMooseObject("raccoonApp", ADSPFPressureTimeDerivative);

InputParameters
ADSPFPressureTimeDerivative::validParams()
{
  InputParameters params = ADTimeDerivative::validParams();
  params.addClassDescription(
      "AD Time derivative of pressure divided by the biot modulus in single phase flow.");
  params.addParam<MaterialPropertyName>("biot_modulus", "biot_modulus",  "biot_modulus");
  return params;
}

ADSPFPressureTimeDerivative::ADSPFPressureTimeDerivative(const InputParameters & parameters)
  : ADTimeDerivative(parameters),
    _M(getMaterialProperty<Real>("biot_modulus"))
{
}

ADReal
ADSPFPressureTimeDerivative::precomputeQpResidual()
{
  return 1/_M[_qp] * ADTimeDerivative::precomputeQpResidual();
}
