[Mesh]
  type = FileMesh
  file = './gold/geo.msh'
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]

[AuxVariables]
  [d]
  []
  [E_el_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [fx]
  []
  [fy]
  []
  [./stress_xx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./stress_xx]
    type = ADRankTwoAux
    variable = 'stress_xx'
    rank_two_tensor = 'stress'
    index_i = 0
    index_j = 0
    execute_on = 'TIMESTEP_END'
  [../]
  [./stress_yy]
    type = ADRankTwoAux
    variable = 'stress_yy'
    rank_two_tensor = 'stress'
    index_i = 1
    index_j = 1
    execute_on = 'TIMESTEP_END'
  [../]
  [E_el]
    type = ADMaterialRealAux
    variable = 'E_el_active'
    property = 'E_el_active'
    execute_on = 'TIMESTEP_END'
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    displacements = 'disp_x disp_y'
    save_in = 'fx'
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    displacements = 'disp_x disp_y'
    save_in = 'fy'
  []
[]

[BCs]
  [ydisp]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'top'
    function = cyclic_disp
    preset = false
  []
  [xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'bottom top'
    value = 0
  []
  [yfix]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'bottom'
    value = 0
  []
[]

[Functions]
  [./cyclic_disp]
    type = PiecewiseLinear
    data_file = 'cyclic-disp.csv'
    format = columns
  [../]
[]

[Materials]
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = ${E}
    poissons_ratio = ${nu}
  []
  [strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  []
  [stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainVolDev
    d = 'd'
  []
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

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'

  dt = 1.0e-4
  end_time = 3.5e-2
  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  automatic_scaling = true
[]

[Postprocessors]
  [Fy]
    type = NodalSum
    variable = 'fy'
    boundary = 'top'
  []
  [strain_energy]
    type = ADStrainEnergy
  []
  [active_strain_energy]
    type = ADElementIntegralMaterialProperty
    mat_prop = 'E_el_active'
  []
  [fracture_energy]
    type = ADFractureEnergy
    d = 'd'
    mobility_name = 'fatigue_mobility'
  []
  [external_energy]
    type = ExternalEnergy
    boundary = 'top'
    displacements = 'disp_x disp_y'
    forces = 'fx fy'
  []
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  [csv]
    type = CSV
    delimiter = ' '
    file_base = 'out_vars'
  []
  [exodus]
    type = Exodus
    file_base = 'solid'
    interval = 5
  []
[]
