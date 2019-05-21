//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "MooseApp.h"

class raccoonTestApp;

template <>
InputParameters validParams<raccoonTestApp>();

class raccoonTestApp : public MooseApp
{
public:
  raccoonTestApp(InputParameters parameters);
  virtual ~raccoonTestApp();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs = false);
};

