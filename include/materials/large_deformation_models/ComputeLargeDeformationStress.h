//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "ADRankTwoTensorForward.h"

class LargeDeformationElasticityModel;
class LargeDeformationPlasticityModel;
class LargeDeformationViscoelasticityModel;

/**
 * ComputeLargeDeformationStress computes the stress given certain hyperelasticity and optionally a
 * plasticity model and a viscoelasticity model
 */
class ComputeLargeDeformationStress : public Material
{
public:
  static InputParameters validParams();

  ComputeLargeDeformationStress(const InputParameters & parameters);

  virtual void initialSetup() override;

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  /// The elasticity model
  LargeDeformationElasticityModel * _elasticity_model;

  /// The elasticity model
  LargeDeformationPlasticityModel * _plasticity_model;

  /// The viscoelasticity model
  LargeDeformationViscoelasticityModel * _viscoelasticity_model;

  /// Base name of the material system
  const std::string _base_name;

  // @{ The mechanical strain excluding eigen strains from the total strain
  const ADMaterialProperty<RankTwoTensor> & _Fm;
  const MaterialProperty<RankTwoTensor> * _Fm_old;
  // @}

  /// The stress
  ADMaterialProperty<RankTwoTensor> & _stress;
};
