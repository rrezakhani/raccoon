//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"

class ADComputeFluidMobility : public Material
{
public:
  static InputParameters validParams();

  ADComputeFluidMobility(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  ADMaterialProperty<RankTwoTensor> & _fluid_mob;
  ADMaterialProperty<Real> & _wn;
  
  /// damage and gradient of damage
  const ADVariableValue & _d;
  const ADVariableGradient & _grad_d;
  
  const ADMaterialProperty<RankTwoTensor> & _strain;
  const MaterialProperty<RankTwoTensor> & _strain_old;
  
  const ADMaterialProperty<Real> & _eta;
  const ADMaterialProperty<Real> & _K;

  const Real & _mob_eps; 
};
