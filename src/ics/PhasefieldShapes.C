/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "PhasefieldShapes.h"

registerMooseObject("raccoonApp",PhasefieldShapes);

template<>
InputParameters validParams<PhasefieldShapes>()
{
  InputParameters params = validParams<InitialCondition>();
  params.addClassDescription("Introduce 2D Phase Field Multiple IC");
  params.addRequiredParam<Real>("epsilon", "Phase field itransition parameter");
  params.addParam<std::vector<std::string> >("shapes", "array of shape types: crack, circle");
  params.addParam<std::vector<std::string> >("gradients", "array of gradient types: box");
  params.addParam<std::vector<Real> >("centers_tips", "coordinates");
  params.addParam<std::vector<Real> >("gradient_values", "coordinates and values");


  return params;
}

PhasefieldShapes::PhasefieldShapes(const InputParameters & parameters) :
    InitialCondition(parameters),
    _epsilon(getParam<Real>("epsilon")),
    _centers_tips(getParam<std::vector<Real> >("centers_tips")),
    _shapes(getParam<std::vector<std::string> >("shapes")),
    _gradients(getParam<std::vector<std::string> >("gradients")),
    _gradient_values(getParam<std::vector<Real> >("gradient_values"))

{
}

Real
PhasefieldShapes::value(const Point & p)
{

int _num_shapes = _shapes.size();
int _num_gradients = _gradients.size();

//for shapes

int steps=1000;
double distance=9e10;//start with big value.
double aux_distance=0.0;
double current_value=0.0;
double _crack_x0, _crack_x1, _crack_y0, _crack_y1;
double _center_x, _center_y, _radius_a; //, _radius_b;
double _x1, _y1, _v1, _x2, _y2, _v2, _v3, _v4;

for (int i=0; i<_num_shapes;i++)
{
if (_shapes[i]=="crack")
{
    _crack_x0 = _centers_tips[i*4+0];
    _crack_y0 = _centers_tips[i*4+1];
    _crack_x1 = _centers_tips[i*4+2];
    _crack_y1 = _centers_tips[i*4+3];

    double step_x = (_crack_x1 - _crack_x0)/steps;
    double step_y = (_crack_y1 - _crack_y0)/steps;

    aux_distance=sqrt((p(0)-(_crack_x0))*(p(0)-(_crack_x0))+(p(1)-(_crack_y0))*(p(1)-(_crack_y0)));
    if (aux_distance<distance) distance=aux_distance;

    for (int j=1;j<steps+1;j++)
    {
    aux_distance=sqrt((p(0)-(_crack_x0 + j*step_x))*(p(0)-(_crack_x0+ j*step_x))+(p(1)-(_crack_y0 + j*step_y))*(p(1)-(_crack_y0 + j*step_y)));
    if (aux_distance<distance) distance=aux_distance;

    current_value = 1.0 - tanh(distance/_epsilon);
    }
}

if (_shapes[i]=="circle")
{


    double amp, k;
    amp = _centers_tips[i*6+4];
    k = _centers_tips[i*6+5];




    _center_x = _centers_tips[i*6+0];
    _center_y = _centers_tips[i*6+1];
    _radius_a = _centers_tips[i*6+2];
    //_radius_b = _centers_tips[i*6+3];


    //outward radial vector
   // double norm = sqrt((p(0)-_center_x)*(p(0)-_center_x)+(p(1)-_center_y)*(p(1)-_center_y));
    //_v1 = (p(0)-_center_x)/norm;
    //_v2 = (p(1)-_center_y)/norm;

   // _v3 = center_x + _radius_a*_v1;
    //_v4 = center_y + _radius_a*_v2;

    //angle

    double pi=3.141591;
    double angle = atan((p(1)-_center_y)/(p(0)-_center_x));


    if (((p(0)-_center_x)>=0.0)&&((p(1)-_center_y)>=0.0)) angle = angle;
    if (((p(0)-_center_x)<0.0)&&((p(1)-_center_y)>=0.0))  angle = pi + angle;
    if (((p(0)-_center_x)<0.0)&&((p(1)-_center_y)<0.0))  angle = pi + angle;
    if (((p(0)-_center_x)>=0.0)&&((p(1)-_center_y)<0.0)) angle = angle;



    double perturbation = amp*sin(k*angle/(2*pi));



    aux_distance=sqrt((p(0)-_center_x)*(p(0)-_center_x)+(p(1)-_center_y)*(p(1)-_center_y));//distance to



    current_value = exp(-(aux_distance-(_radius_a+perturbation))/_epsilon);
    if (aux_distance<(_radius_a+perturbation)) current_value=1.0;//if negative then is interior, then assign 1.0;




    }


    if (_shapes[i]=="line")
    {

        //points
        _x1 = _centers_tips[i*6+0];
        _y1 = _centers_tips[i*6+1];
        _x2 = _centers_tips[i*6+2];
        _y2 = _centers_tips[i*6+3];

        //vector
        double norm = sqrt((_x2-_x1)*(_x2-_x1)+(_y2-_y1)*(_y2-_y1));
        _v1 = (_x2-_x1)/norm;
        _v2 = (_y2-_y1)/norm;
        //normal
        _v3 = -_v2;
        _v4 = _v1;

        double pi=3.141591;

        distance = (p(0)-_x1)*_v3+(p(1)-_y1)*_v4;

        double amp, k;
        amp = _centers_tips[i*6+4];
        k = _centers_tips[i*6+5];

        double perturbation = amp*sin(((p(0)-_x1)*_v1+(p(1)-_y1)*_v2)/k*pi);


        current_value = 0.5*(1.0 + tanh(((distance+perturbation)/_epsilon)));






    }


}

for (int i=0; i<_num_gradients;i++)
{
if (_gradients[i]=="box")
{
    _x1 = _gradient_values[i*8+0];
    _y1 = _gradient_values[i*8+1];
    _x2 = _gradient_values[i*8+2];
    _y2 = _gradient_values[i*8+3];
    _v1 = _gradient_values[i*8+4];
    _v2 = _gradient_values[i*8+5];
    _v3 = _gradient_values[i*8+6];
    _v4 = _gradient_values[i*8+7];


    double d_x[2], d_y[2];
    d_x[0]=_x2-p(0);
    d_x[1]=p(0)-_x1;
    d_y[0]=_y2-p(1);
    d_y[1]=p(1)-_y1;


     current_value = (1/(_x2-_x1)/(_y2-_y1))*((d_x[0]*_v1+d_x[1]*_v2)*d_y[0]+(d_x[0]*_v3+d_x[1]*_v4)*d_y[1]);

}
    if (_gradients[i]=="box_pattern")
    {
        _x1 = _gradient_values[i*8+0];
        _y1 = _gradient_values[i*8+1];
        _x2 = _gradient_values[i*8+2];
        _y2 = _gradient_values[i*8+3];
        _v1 = _gradient_values[i*8+4];
        _v2 = _gradient_values[i*8+5];
        _v3 = _gradient_values[i*8+6];
        _v4 = _gradient_values[i*8+7];


        double d_x, d_y;

        d_x=p(0)-_x1;
        d_y=p(1)-_y1;

        double pi=3.141591;


        current_value = _v1+(_v2-_v1)*std::abs(sin(pi*d_x/_v3))*std::abs(sin(pi*d_y/_v4));

    }
if (_gradients[i]=="line")
{
    _x1 = _gradient_values[i*8+0];
    _y1 = _gradient_values[i*8+1];
    _x2 = _gradient_values[i*8+2];
    _y2 = _gradient_values[i*8+3];
    _v1 = _gradient_values[i*8+4];
    _v2 = _gradient_values[i*8+5];
    _v3 = _gradient_values[i*8+6];
    _v4 = _gradient_values[i*8+7];


    double m=(_y2-_y1)/(_x2-_x1);


    if (p(1)<(_y1+m*(p(0)-_x1)))
    {

     double distance_1 = (_y1+m*(p(0)-_x1))-p(1);
     double diff_value = (_v1+_v2)/2-_v1;
     current_value = _v1 + exp(-distance_1/_epsilon)*diff_value;
    }

    if (p(1)>=(_y1+m*(p(0)-_x1)))
    {

     double distance_1 = -(_y1+m*(p(0)-_x1))+p(1);
     double diff_value = (_v1+_v2)/2-_v2;
     current_value = _v2 + exp(-distance_1/_epsilon)*diff_value;
    }


}
}


  return current_value;
}
