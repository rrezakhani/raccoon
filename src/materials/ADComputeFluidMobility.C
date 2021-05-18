//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADComputeFluidMobility.h"
#include "RankTwoTensor.h"

registerMooseObject("raccoonApp", ADComputeFluidMobility);

InputParameters
ADComputeFluidMobility::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("compute mobility in porous media including effect of damage");
  params.addParam<MaterialPropertyName>(
      "fluid_mobility_name", "fluid_mobility", "Name of fluid mobility material property");
  params.addParam<MaterialPropertyName>(
      "crack_opening_name", "crack_opening", "Name of the crack opening variable");
  params.addRequiredCoupledVar("damage", "damage field");
  params.addParam<MaterialPropertyName>(
      "fluid_viscosity_name", "fluid_viscosity",  "fluid_viscosity");
  params.addParam<MaterialPropertyName>(
      "fluid_permeability_name", "fluid_permeability",  "fluid_permeability");
  params.addRequiredParam<Real>("mobility_exponent", "exponent used in mobility equation");
  return params;
}

ADComputeFluidMobility::ADComputeFluidMobility(const InputParameters & parameters)
  : Material(parameters),
    _fluid_mob(declareADProperty<RankTwoTensor>(getParam<MaterialPropertyName>("fluid_mobility_name"))),
    _wn(declareADProperty<Real>(getParam<MaterialPropertyName>("crack_opening_name"))),
    _d(adCoupledValue("damage")),
    _grad_d(adCoupledGradient("damage")),
    _strain(getADMaterialPropertyByName<RankTwoTensor>("total_strain")),
    _strain_old(getMaterialPropertyOldByName<RankTwoTensor>("total_strain")),
    _eta(getADMaterialProperty<Real>("fluid_viscosity_name")),
    _K(getADMaterialProperty<Real>("fluid_permeability_name")),
    _mob_eps(getParam<Real>("mobility_exponent"))
{
}

void
ADComputeFluidMobility::computeQpProperties()
{
  const Real eps = libMesh::TOLERANCE;
  ADRealVectorValue n;
  if (_grad_d[_qp].norm() > eps)
     n = _grad_d[_qp] / _grad_d[_qp].norm();
  //_wn[_qp] = _current_elem->hmin() * (_strain[_qp] * n) * n;
  _wn[_qp] = _current_elem->hmin() * (_strain_old[_qp] * n) * n;

  ADRankTwoTensor identity(ADRankTwoTensor::initIdentity);
  ADRankTwoTensor matrix_mob = _K[_qp]/_eta[_qp] * identity;
  ADRankTwoTensor nn;
  nn.vectorOuterProduct(n,n);
  //ADRankTwoTensor fracture_mob = (_wn[_qp]*_wn[_qp]/(12*_eta[_qp]) - _K[_qp]/_eta[_qp]) * (identity - nn);
  //ADRankTwoTensor fracture_mob = _wn[_qp]*_wn[_qp]/(12*_eta[_qp]) * (identity - nn);
  ADRankTwoTensor fracture_mob = std::abs(_wn[_qp])/(12*_eta[_qp]) * (identity - nn);

  _fluid_mob[_qp] = matrix_mob + std::pow(_d[_qp],_mob_eps) * fracture_mob;
}
