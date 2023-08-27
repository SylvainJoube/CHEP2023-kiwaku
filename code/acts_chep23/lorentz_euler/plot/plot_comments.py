

# if use_acts_field:
#   lorentz_covfie_list     = load_file("data3/lorentz-euler_covfie_blop-debian11_4096-65536_1024-65536_128-128_InterpolateLin-LayoutStride-field.txt")
#   lorentz_standalone_list = load_file("data3/lorentz_standalone_opti_blop-debian11_4096-65536_1024-65536_128-128_acts-field.txt")
#   lorentz_kiwaku_list     = load_file("data3/lorentz_kiwaku_opti_blop-debian11_4096-65536_1024-65536_128-128_acts-field.txt")
# else:
#   lorentz_covfie_list     = load_file("data/lorentz-euler_covfie_blop-debian11_2023-04-17_4096-65536_1024-2048_128-131072_constant-field.txt")
#   lorentz_standalone_list = load_file("data3/lorentz_standalone_blop-debian11_2023-04-16_4096-65536_1024-2048_128-131072_constant-field.txt")
#   lorentz_kiwaku_list     = load_file("data3/lorentz_kiwaku_blop-debian11_2023-04-16_4096-65536_1024-2048_128-131072_constant-field.txt")

# Constant field: msi-chaton
# lorentz_covfie_list     = load_file("data/lorentz-euler_covfie_blop-debian11_2023-04-17_4096-65536_1024-2048_128-131072_constant-field.txt")
# lorentz_standalone_list = load_file("data/lorentz_standalone_msi-chaton_4096-65536_1024-2048_128-131072_constant-field.txt")
# lorentz_kiwaku_list     = load_file("data/lorentz_kiwaku_msi-chaton_4096-65536_1024-2048_128-131072_constant-field.txt")


# ACTS field linear
# lorentz_covfie_list     = load_file("data/lorentz-euler_covfie_blop-debian11_2023-04-17_4096-65536_1024-2048_128-131072_InterpolateLin-LayoutStride-field.txt")
# lorentz_standalone_list = load_file("data/lorentz_standalone_blop-debian11_2023-04-16_4096-65536_1024-2048_128-131072_acts-field.txt")
# lorentz_kiwaku_list = load_file("data/lorentz_kiwaku_blop-debian11_2023-04-18_4096-65536_1024-2048_128-131072_acts-field.txt")

# Constant field


# # draw_curve("USM shared copy", "blue", y_list_shared_copy, y_median_shared_copy, "dotted")
# # draw_curve("USM host copy", "red", y_list_host_copy, y_median_host_copy, "dashdot")
# draw_curve("USM shared", "blue", y_list_shared_direct, y_median_shared_direct, "dotted")
# draw_curve("USM host", "red", y_list_host_direct, y_median_host_direct, "dashdot")

# plt.title("SparseCCL - flat arrays")
# #draw_curve("USM device", "green", y_list_device, y_median_device, "solid")
# draw_curve("shared flat", "blue", y_list_shared_flat, y_median_shared_flat, "solid")
# draw_curve("shared ptr", "green", y_list_shared_ptr, y_median_shared_ptr, "dotted")
# #draw_curve("USM host", "maroon", y_list_acc, y_median_acc, "dashed")
# #draw_curve("USM host", "maroon", y_list_host, y_median_host, "dashed")

# Faire l'affichage du tableau de valeurs (x100) et voir ce que ça donne
# par rapport aux précédentes valeurs 

# if FLAT_ONLY:
#     plt.figure(figsize=(800/my_dpi, 800/my_dpi), dpi=my_dpi)
# else:
#     plt.figure(figsize=(800/my_dpi, 800/my_dpi), dpi=my_dpi)


