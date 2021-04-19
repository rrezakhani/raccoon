//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#define _USE_MATH_DEFINES
#include <cmath>
#include "DiracSource.h"
#include "Executioner.h"

registerMooseObject("raccoonApp", DiracSource);

template <>
InputParameters
validParams<DiracSource>()
{
  InputParameters params = validParams<DiracKernel>();
  params.addRequiredParam<Point>("point", "The x,y,z coordinates of the point");
  params.addRequiredParam<int>("dim", "The dimension of the problem");
  params.addRequiredParam<Real>("fL", "some coefficient");
  params.addRequiredParam<Real>("t1", "some coefficient");
  params.addRequiredParam<Real>("tRT", "some coefficient");
  params.addRequiredParam<Real>("tL", "some coefficient");
  params.addRequiredParam<Real>("tP", "some coefficient");
  params.addRequiredParam<Real>("p0", "some coefficient");
  params.addRequiredParam<Real>("d1", "some coefficient");
  params.addRequiredParam<Real>("upcoeff", "some coefficient");
  params.addRequiredParam<Real>("downcoeff", "some coefficient");
  params.addRequiredParam<Real>("rho", "some coefficient");
  params.addRequiredParam<Real>("num_shots", "number of shots");
  //params.addParam<MaterialPropertyName>("density_name","density", "name of the density material");
  return params;
}

DiracSource::DiracSource(const InputParameters & parameters)
  : DiracKernel(parameters),
    _point(getParam<Point>("point")),
    _dim(getParam<int>("dim")),
    _fL(getParam<Real>("fL")),
    _t1(getParam<Real>("t1")),
    _tRT(getParam<Real>("tRT")),
    _tL(getParam<Real>("tL")),
    _tP(getParam<Real>("tP")),
    _p0(getParam<Real>("p0")),
    _d1(getParam<Real>("d1")),
    _upcoeff(getParam<Real>("upcoeff")),
    _downcoeff(getParam<Real>("downcoeff")),
    _rho(getParam<Real>("rho")),
    _num_shots(getParam<Real>("num_shots"))
    //_mat(getMaterialProperty<Real>("density_name"))
{
}

void
DiracSource::addPoints()
{
  // Add a point from the input file
  addPoint(_point);
}

Real
DiracSource::computeQpResidual()
{
  const Real _end_t = _app.getExecutioner()->getParam<Real>("end_time");
  Real _shot_duration = _end_t/_num_shots;
  int _current_shot = std::floor(_t/_shot_duration);
  Real _shot_t = _t - _current_shot*_shot_duration;

  // Yingjie's paper
  Real _pressure_source = 1/_tP*4*M_PI/_rho*_upcoeff/_downcoeff*_p0*_d1*(std::max((1+tanh((_shot_t-_t1)/_tRT))*std::exp(-(_shot_t-_t1)/_tL)*cos(2*M_PI*_fL*(_shot_t-_t1)+M_PI/3),0.0)); 
  
  // Zhang's thesis
  //Real _pressure_source = 1/_tP*4*M_PI/_rho*_upcoeff/_downcoeff*_p0*_d1*(1+tanh((_t-_t1)/_tRT))*std::exp(-(_t-_t1)/_tL)*cos(2*M_PI*_fL*(_t-_t1)+M_PI/3); 
  
  // Half sinus wave
  //Real _pressure_source = _p0*std::max(sin(2*M_PI*_t/_end_t),0.0);

  // Constant pressure 
  //Real _pressure_source = _p0;
  
  //std::ofstream outfile;
  //outfile.open("pressure_source.csv", std::ios_base::app);
  //outfile.precision(5);
  //outfile << _t << ", " << _current_shot+1 << "," << _pressure_source << std::endl;
  //outfile.close();
  
  // source pressure times test function is returned
  return -_test[_i][_qp] * _pressure_source;
}

