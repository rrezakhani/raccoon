//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADAnisotropicDiffusion.h"

registerMooseObject("raccoonApp", ADAnisotropicDiffusion);

InputParameters
ADAnisotropicDiffusion::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription(
      "Anisotropic diffusion of fluid in fractured media");
  return params;
}

ADAnisotropicDiffusion::ADAnisotropicDiffusion(const InputParameters & parameters)
  : ADKernel(parameters), 
  _fluid_mob(getADMaterialProperty<RankTwoTensor>("fluid_mobility"))
{
}

ADReal
ADAnisotropicDiffusion::computeQpResidual()
{
  return (_fluid_mob[_qp] * _grad_u[_qp]) * _grad_test[_i][_qp];
}

