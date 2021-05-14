//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"

class ADPoroMechanicsCoupling : public ADKernel
{
public:
  static InputParameters validParams();

  ADPoroMechanicsCoupling(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  const ADMaterialProperty<Real> & _coefficient;

  const VariableValue & _porepressure;

  unsigned int _porepressure_var_num;

  unsigned int _component;
};
