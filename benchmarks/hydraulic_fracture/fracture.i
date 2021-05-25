###############################################################################
E = 16
nu = 0.1812
Gc = 9.49e-9
l = 0.75
psic = 7.812e-9
k = 1e-3

###############################################################################
[Mesh]
  [./generated_mesh]
    type = GeneratedMeshGenerator
    dim = 2
    elem_type = QUAD4
    nx = 80
    ny = 40
    nz = 0
    xmin = 0
    xmax = 40
    ymin = 0
    ymax = 20
    zmin = 0
    zmax = 0
  [../]
[]

[ICs]
  [./d]
    type = CohesiveDamageIC
    variable = d
    d0 = 1.0
    l = ${l}
    x1 = 16.0
    y1 = 10.0
    z1 = 0.0
    x2 = 24.0
    y2 = 10.0
    z2 = 0
  [../]
[]

###############################################################################
[MultiApps]
  [mechanical]
    type = TransientMultiApp
    input_files = 'poromechanics.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    cli_args = 'E=${E};nu=${nu};Gc=${Gc};l=${l};psic=${psic};k=${k}'
  []
[]

[Transfers]
  [get_E_el_active]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = from_multiapp
    source_variable = 'E_el_active'
    variable = 'E_el_active'
  []
  [send_d]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'd'
    variable = 'd'
  []
[]

[Variables]
  [d]
  []
[]

[AuxVariables]
  [E_el_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [bounds_dummy]
  []
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = lower
  []
  [upper]
    type = ConstantBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = upper
    bound_value = 1
  []
[]

[Kernels]
  [diff]
    type = ADPFFDiffusion
    variable = 'd'
  []
  [barrier]
    type = ADPFFBarrier
    variable = 'd'
  []
  [react]
    type = ADPFFReaction
    variable = 'd'
    driving_energy_var = 'E_el_active'
  []
[]

[Materials]
  [fracture_bulk]
    type = ADGenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length critical_fracture_energy'
    prop_values = '${Gc} ${l} ${psic}'
  []
  [./local_dissipation]
    type = QuadraticLocalDissipation
    d = 'd'
  [../]
  [./fracture_properties]
    type = ADFractureMaterial
    local_dissipation_norm = 2
  [../]
  [./degradation]
    type = QuadraticDegradation
    d = 'd'
    residual_degradation = ${k}
  [../]
[]

[Postprocessors]
  [norm_d]
    type = ElementL2Norm
    variable = 'd'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu       vinewtonrsls'

  dt = 1e-1
  end_time = 5
  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  picard_max_its = 20
  disable_picard_residual_norm_check = true
  picard_custom_pp = 'norm_d'
  custom_abs_tol = 1e-03
  custom_rel_tol = 1e-03
  accept_on_max_picard_iteration = false
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
[]
