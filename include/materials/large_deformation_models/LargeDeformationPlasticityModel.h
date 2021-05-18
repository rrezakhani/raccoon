//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "ADRankTwoTensorForward.h"
#include "ADSingleVariableReturnMappingSolution.h"
#include "BaseNameInterface.h"
#include "PlasticHardeningModel.h"

class LargeDeformationElasticityModel;

class LargeDeformationPlasticityModel : public Material,
                                        public ADSingleVariableReturnMappingSolution,
                                        public BaseNameInterface
{
public:
  static InputParameters validParams();

  LargeDeformationPlasticityModel(const InputParameters & parameters);

  virtual void initialSetup() override;

  /// Set the current quadrature point
  virtual void setQp(unsigned int qp);

  /// Set the associated elasticity model
  virtual void setElasticityModel(LargeDeformationElasticityModel * elasticity_model);

  /**
   * Update the stress and elastic deformation gradient if need to following the specified plastic
   * flow
   * @param stress  The stress
   * @param Fe      The elastic deformation gradient
   */
  virtual void updateState(ADRankTwoTensor & stress, ADRankTwoTensor & Fe) = 0;

  // @{ Retained as empty methods to avoid a warning from Material.C in framework. These methods are
  // unused in all inheriting classes and should not be overwritten.
  void resetQpProperties() final {}
  void resetProperties() final {}
  // @}

protected:
  virtual void initQpStatefulProperties() override;

  /// The elasticity model
  LargeDeformationElasticityModel * _elasticity_model;

  /// The plastic deformation gradient
  ADMaterialProperty<RankTwoTensor> & _Fp;
  const MaterialProperty<RankTwoTensor> & _Fp_old;

  /// The (scalar) effective plastic strain
  ADMaterialProperty<Real> & _ep;
  const MaterialProperty<Real> & _ep_old;

  /// The flow direction
  ADMaterialProperty<RankTwoTensor> & _Np;

  /// The plastic hardening model
  PlasticHardeningModel * _hardening_model;
};
