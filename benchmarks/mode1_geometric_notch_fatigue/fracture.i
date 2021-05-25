E = 2.1e5
nu = 0.3
Gc = 2.7
l = 0.015
k = 1e-09
alphaT = 30.0

[MultiApps]
  [mechanical]
    type = TransientMultiApp
    input_files = 'mechanical.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    cli_args = 'E=${E};nu=${nu};Gc=${Gc};l=${l};k=${k};alphaT=${alphaT}'
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

[Mesh]
  type = FileMesh
  file = './gold/geo.msh'
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
  [./alpha_bar]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./f_alpha]
    order = CONSTANT
    family = MONOMIAL
  [../]
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
    mobility_name = 'fatigue_mobility'
  []
  [barrier]
    type = ADPFFBarrier
    variable = 'd'
    mobility_name = 'fatigue_mobility'
  []
  [react]
    type = ADPFFReaction
    variable = 'd'
    driving_energy_var = 'E_el_active'
  []
[]

[AuxKernels]
  [./alpha_bar]
    type = ADMaterialRealAux
    property = 'alpha_bar'
    variable = 'alpha_bar'
    execute_on = 'TIMESTEP_END'
  [../]
  [./f_alpha]
    type = ADMaterialRealAux
    property = 'f_alpha'
    variable = 'f_alpha'
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Materials]
  [fracture_bulk]
    type = ADGenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length'
    prop_values = '${Gc} ${l}'
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
  [./fatigue_mobility]
    type = FatigueMobility
    elastic_energy_var = E_el_active
    f_alpha_type = 'asymptotic'
    alpha_T = ${alphaT}
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

  dt = 1.0e-4
  end_time = 3.5e-2
  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  picard_max_its = 100
  disable_picard_residual_norm_check = true
  picard_custom_pp = 'norm_d'
  custom_abs_tol = 1e-50
  custom_rel_tol = 1e-08
  accept_on_max_picard_iteration = true
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  [exodus]
    type = Exodus
    file_base = 'damage'
    interval = 5
  []
[]
