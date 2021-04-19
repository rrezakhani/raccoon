//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"
#include "MaterialPropertyUserObject.h"

class FatigueMobility : public ADMaterial
{
public:
  static InputParameters validParams();

  FatigueMobility(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  /// base name
  const std::string _base_name;

  /// mobility and fatigue mobility
  ADMaterialProperty<Real> & _M;
  const ADMaterialProperty<Real> & _M0;

  /// degradation function
  const ADMaterialProperty<Real> & _w;

  /// elastic energy name
  const MaterialPropertyName _E_el_name;
  
  /// active part of elastic energy 
  const ADMaterialProperty<Real> * _E_el_active;
  const MaterialProperty<Real> * _E_el_active_old;
  
  /// active part of elastic energy from coupled variable 
  const VariableValue * _E_el_active_var;
  const VariableValue * _E_el_active_var_old;
  
  /// fatigue history variable
  ADMaterialProperty<Real> & _alpha_bar;

  /// old fatigue history variable
  const MaterialProperty<Real> & _alpha_bar_old;
  
  /// fatigue degradation function
  ADMaterialProperty<Real> & _f_alpha;
  
  /// fatigue degradation function type 
  const MaterialPropertyName _f_alpha_type;
  
  /// parameter in logaritmic fatigue degradation function
  const Real & _kappa;
  
  /// fatigue threshold
  const Real & _alpha_T;

  /// initial fatigue history variable (alpha_bar)
  const VariableValue * _alpha_bar_init; 

  /// degradation function and its old value
  const ADMaterialProperty<Real> & _g;
  const MaterialProperty<Real> & _g_old;

  MaterialProperty<bool> & _fatigue_flag;
};
