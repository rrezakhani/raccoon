//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADTimeKernel.h"

class ADSPFVolStrTimeDerivative : public ADTimeKernel
{
public:
  static InputParameters validParams();

  ADSPFVolStrTimeDerivative(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;
  
  const ADMaterialProperty<RankTwoTensor> & _strain;
  const MaterialProperty<RankTwoTensor> & _strain_old;
  
  const MaterialProperty<Real> & _alpha;
};
