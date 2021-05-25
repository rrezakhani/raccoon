###############################################################################
eta = 1e-12
k_int = 2e-12
Q = 1e-3
alpha = 0.79
eps = 50
M = 12.5

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
  [./d0]
    type = CohesiveDamageIC
    variable = d0
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
[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./pressure]
  [../]
[]

[AuxVariables]
  [./d0]
  [../]
  [./d]
  [../]
  [./E_el_active]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_h]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_xx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_xy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./lambda_xx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./lambda_xy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./lambda_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./crack_opening]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./fluid_source_term]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./biot_coefficient]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

###############################################################################
[BCs]
  [./xfix]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'top bottom left right'
    value = 0
  [../]
  [./yfix]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'top bottom left right'
    value = 0
  [../]
  [./p]
    type = DirichletBC
    variable = 'pressure'
    boundary = 'top bottom left right'
    value = 0
  [../]
[]

###############################################################################
[Kernels]
  # solid mechaincs
  ###################################
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    displacements = 'disp_x disp_y'
    component = 0
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    displacements = 'disp_x disp_y'
    component = 1
  [../]
  [./poro_x]
    type = ADPoroMechanicsCoupling
    variable = 'disp_x'
    porepressure = 'pressure'
    component = 0
  [../]
  [./poro_y]
    type = ADPoroMechanicsCoupling
    variable = 'disp_y'
    porepressure = 'pressure'
    component = 1
  [../]

  # pressure
  ###################################
  [./timederivative_pressure]
    type =  ADSPFPressureTimeDerivative
    variable = pressure
  [../]
  [./timederivative_epsV]
    type = ADSPFVolStrTimeDerivative
    variable = pressure
  [../]
  [./diffusion_pressure]
    type =  ADAnisotropicDiffusion
    variable = pressure
  [../]
  [./source_term]
    type = ADCoefMatSource
    variable = pressure
    prop_names = 'fluid_source_term'
    coefficient = -1
  [../]
  ###################################

[]

###############################################################################
[AuxKernels]
  [./E_el]
    type = ADMaterialRealAux
    variable = 'E_el_active'
    property = 'E_el_active'
    execute_on = 'TIMESTEP_END'
  []
  [./stress_h]
    type = ADRankTwoScalarAux
    rank_two_tensor = 'stress'
    variable = 'stress_h'
    scalar_type = Hydrostatic
    execute_on = 'TIMESTEP_END'
  [../]
  [./stress_xx]
    type = ADRankTwoAux
    variable = 'stress_xx'
    rank_two_tensor = 'stress'
    index_i = 0
    index_j = 0
    execute_on = 'TIMESTEP_END'
  [../]
  [./stress_xy]
    type = ADRankTwoAux
    variable = 'stress_xy'
    rank_two_tensor = 'stress'
    index_i = 0
    index_j = 1
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
  [./lambda_xx]
    type = ADMaterialRankTwoTensorAux
    property = 'fluid_mobility'
    variable = 'lambda_xx'
    i = 0
    j = 0
    execute_on = 'TIMESTEP_END'
  [../]
  [./lambda_xy]
    type = ADMaterialRankTwoTensorAux
    property = 'fluid_mobility'
    variable = 'lambda_xy'
    i = 0
    j = 1
    execute_on = 'TIMESTEP_END'
  [../]
  [./lambda_yy]
    type = ADMaterialRankTwoTensorAux
    property = 'fluid_mobility'
    variable = 'lambda_yy'
    i = 1
    j = 1
    execute_on = 'TIMESTEP_END'
  [../]
  [./crack_opening]
    type = ADMaterialRealAux
    property = 'crack_opening'
    variable = 'crack_opening'
    execute_on = 'TIMESTEP_END'
  [../]
  [./fluid_source_term]
    type = ADMaterialRealAux
    property = 'fluid_source_term'
    variable = 'fluid_source_term'
    execute_on = 'TIMESTEP_END'
  [../]
  [./biot_coefficient]
    type = ADMaterialRealAux
    property = 'biot_coefficient'
    variable = 'biot_coefficient'
    execute_on = 'TIMESTEP_END'
  [../]
[]

###############################################################################
[Materials]
  [./constants]
    type = ADGenericConstantMaterial
    prop_names = 'fluid_viscosity fluid_permeability fluid_source'
    prop_values = '${eta} ${k_int} ${Q}'
  [../]

  [./biot_intact]
    type = ADGenericConstantMaterial
    prop_names = 'intact_biot_coef'
    prop_values = '${alpha}'
  [../]

  [./mob_w]
    type = ADComputeFluidMobility
    damage = 'd'
    mobility_exponent = '${eps}'
  [../]

  [./fluid_source_term]
    type = ADParsedMaterial
    f_name = fluid_source_term
    material_property_names = 'fluid_source'
    args = 'd0'
    function = 'if(d0>0.5, fluid_source, 0.0)'
  [../]

  [./biot_coefficient]
    type = ADDerivativeParsedMaterial
    f_name = biot_coefficient
    material_property_names = 'intact_biot_coef'
    args = 'd'
    function = 'intact_biot_coef'
    enable_jit = true
    derivative_order = 1
  [../]

  [./elast_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = ${E}
    poissons_ratio = ${nu}
  [../]
  [./strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainSpectral
    d = 'd'
  []
  [fracture_properties]
    type = ADGenericFunctionMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length critical_fracture_energy'
    prop_values = '${Gc} ${l} ${psic}'
  []
  [./local_dissipation]
    type = QuadraticLocalDissipation
    d = 'd'
  [../]
  [./phase_field_properties]
    type = ADFractureMaterial
    local_dissipation_norm = 2
  [../]
  [./degradation]
    type = QuadraticDegradation
    d = 'd'
    residual_degradation = ${k}
  [../]
  [./biot]
    type = GenericConstantMaterial
    prop_names = 'biot_modulus'
    prop_values = '${M}'
  [../]
[]

###############################################################################
[Postprocessors]
  [./step_size]             # Size of the time step
    type = TimestepSize
  [../]
  [./iterations]            # Number of iterations needed to converge timestep
    type = NumNonlinearIterations
  [../]
  [./nodes]                 # Number of nodes in mesh
    type = NumNodes
  [../]
  [./evaluations]           # Cumulative residual calculations for simulation
    type = NumResidualEvaluations
  [../]
  [./active_time]           # Time computer spent on simulation
    type = PerfGraphData
    section_name = "Root"
    data_type = total
  [../]
[]

###############################################################################
[Preconditioning]
  [./coupled]
    type = SMP
    full = true
  [../]
[]

###############################################################################
[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  nl_max_its = 50
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  dt = 1e-1
  end_time = 5
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  automatic_scaling = true
[]

###############################################################################
[Outputs]
  file_base = chtest
  interval = 1
  exodus = true
  console = true
  csv = true
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  print_linear_residuals = false
  [./console]
    type = Console
    max_rows = 10
  [../]
[]

###############################################################################