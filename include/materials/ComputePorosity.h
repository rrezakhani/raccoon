//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"

class ComputePorosity : public Material
{
public:
  static InputParameters validParams();

  ComputePorosity(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  const VariableValue & _porepressure;
  const MaterialProperty<RankTwoTensor> & _strain;

  MaterialProperty<Real> & _porosity;
  const MaterialProperty<Real> & _porosity_old;
  const Real _initial_porosity;

  const MaterialProperty<Real> & _alpha;
  const MaterialProperty<Real> & _M;
};
