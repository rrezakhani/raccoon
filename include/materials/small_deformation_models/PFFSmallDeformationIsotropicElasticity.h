//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "SmallDeformationIsotropicElasticity.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class PFFSmallDeformationIsotropicElasticity : public SmallDeformationIsotropicElasticity,
                                               public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PFFSmallDeformationIsotropicElasticity(const InputParameters & parameters);

  virtual ADRankTwoTensor computeStress(const ADRankTwoTensor & strain) override;

protected:
  // @{ Decomposition methods
  virtual ADRankTwoTensor computeStressNoDecomposition(const ADRankTwoTensor & strain);
  virtual ADRankTwoTensor computeStressSpectralDecomposition(const ADRankTwoTensor & strain);
  virtual ADRankTwoTensor computeStressVolDevDecomposition(const ADRankTwoTensor & strain);
  // @}

private:
  /// Name of the phase-field variable
  const VariableName _d_name;

  // @{ Strain energy density and its derivative w/r/t damage
  const MaterialPropertyName _we_name;
  ADMaterialProperty<Real> & _we;
  ADMaterialProperty<Real> & _we_active;
  ADMaterialProperty<Real> & _dwe_dd;
  // @}

  // @{ The degradation function and its derivative w/r/t damage
  const MaterialPropertyName _g_name;
  const ADMaterialProperty<Real> & _g;
  const ADMaterialProperty<Real> & _dg_dd;
  // @}

  /// Decomposittion types
  const enum class Decomposition { none, spectral, voldev } _decomposition;
};
