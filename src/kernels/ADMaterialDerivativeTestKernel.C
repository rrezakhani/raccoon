//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADMaterialDerivativeTestKernel.h"

registerMooseObject("raccoonApp", ADMaterialDerivativeTestKernel);

InputParameters
ADMaterialDerivativeTestKernel::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription("Class used for testing derivatives of a scalar material property.");
  params.addRequiredParam<MaterialPropertyName>(
      "material_property", "Name of material property for which derivatives are to be tested.");
  return params;
}

ADMaterialDerivativeTestKernel::ADMaterialDerivativeTestKernel(const InputParameters & parameters)
  : ADKernelValue(parameters),
    _p(getADMaterialProperty<Real>("material_property"))
{
}

ADReal
ADMaterialDerivativeTestKernel::precomputeQpResidual()
{
  return _p[_qp];
}
