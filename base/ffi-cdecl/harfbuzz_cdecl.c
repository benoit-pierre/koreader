// CPPFLAGS="-I/usr/include/freetype2"

#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb-ot.h>
#include "ffi-cdecl.h"

// The few bits we actually rely on for ffi/harfbuzz.lua
cdecl_c99_type(hb_codepoint_t, uint32_t)
cdecl_union(_hb_var_int_t)
cdecl_type(hb_var_int_t)

cdecl_type(hb_face_t)
cdecl_type(hb_language_t)
cdecl_type(hb_ot_name_id_t)
cdecl_struct(hb_ot_name_entry_t)
cdecl_type(hb_ot_name_entry_t)
cdecl_type(hb_set_t)

cdecl_func(hb_ot_name_list_names)
cdecl_func(hb_language_to_string)
cdecl_func(hb_ot_name_get_utf8)
cdecl_func(hb_set_create)
cdecl_func(hb_face_collect_unicodes)
cdecl_func(hb_set_set)
cdecl_func(hb_set_intersect)
cdecl_func(hb_set_get_population)
cdecl_func(hb_set_destroy)
cdecl_func(hb_face_destroy)
cdecl_func(hb_set_add_range)

// And in front (frontend/fontlist.lua)
cdecl_type(hb_blob_t)
cdecl_type(hb_memory_mode_t)
cdecl_type(hb_destroy_func_t)

cdecl_const(HB_OT_NAME_ID_FONT_FAMILY)
cdecl_const(HB_OT_NAME_ID_FONT_SUBFAMILY)
cdecl_const(HB_OT_NAME_ID_FULL_NAME)

cdecl_type(FT_Face)

cdecl_func(hb_blob_create)
cdecl_func(hb_face_create)
cdecl_func(hb_blob_destroy)
cdecl_func(hb_face_get_glyph_count)
cdecl_func(hb_ft_face_create_referenced)

// The full API follows
/*
cdecl_type(hb_bool_t)
cdecl_c99_type(hb_codepoint_t, uint32_t)
cdecl_c99_type(hb_position_t, int32_t)
cdecl_c99_type(hb_mask_t, uint32_t)
cdecl_c99_type(hb_tag_t, uint32_t)
cdecl_c99_type(hb_color_t, uint32_t)
cdecl_union(_hb_var_int_t)
cdecl_type(hb_var_int_t)
cdecl_type(hb_direction_t)
cdecl_type(hb_language_t)
cdecl_type(hb_script_t)
cdecl_struct(hb_user_data_key_t)
cdecl_type(hb_user_data_key_t)
cdecl_struct(hb_feature_t)
cdecl_type(hb_feature_t)
cdecl_struct(hb_variation_t)
cdecl_type(hb_variation_t)
cdecl_type(hb_memory_mode_t)
cdecl_type(hb_unicode_general_category_t)
cdecl_type(hb_unicode_combining_class_t)
cdecl_type(hb_glyph_flags_t)
cdecl_type(hb_buffer_content_type_t)
cdecl_type(hb_buffer_flags_t)
cdecl_type(hb_buffer_cluster_level_t)
cdecl_type(hb_buffer_serialize_flags_t)
cdecl_type(hb_buffer_serialize_format_t)
cdecl_type(hb_buffer_diff_flags_t)

cdecl_type(hb_blob_t)
cdecl_type(hb_unicode_funcs_t)
cdecl_type(hb_set_t)
cdecl_type(hb_face_t)
cdecl_type(hb_font_t)
cdecl_type(hb_font_funcs_t)
cdecl_struct(hb_font_extents_t)
cdecl_type(hb_font_extents_t)
cdecl_struct(hb_glyph_extents_t)
cdecl_type(hb_glyph_extents_t)
cdecl_struct(hb_glyph_info_t)
cdecl_type(hb_glyph_info_t)
cdecl_struct(hb_glyph_position_t)
cdecl_type(hb_glyph_position_t)
cdecl_struct(hb_segment_properties_t)
cdecl_type(hb_segment_properties_t)
cdecl_type(hb_buffer_t)
cdecl_type(hb_map_t)
cdecl_type(hb_shape_plan_t)

cdecl_type(hb_font_get_font_h_extents_func_t)
cdecl_type(hb_font_get_font_v_extents_func_t)
cdecl_type(hb_font_get_glyph_h_advance_func_t)
cdecl_type(hb_font_get_glyph_v_advance_func_t)
cdecl_type(hb_font_get_glyph_h_advances_func_t)
cdecl_type(hb_font_get_glyph_v_advances_func_t)
cdecl_type(hb_font_get_glyph_h_origin_func_t)
cdecl_type(hb_font_get_glyph_v_origin_func_t)
cdecl_type(hb_font_get_glyph_h_kerning_func_t)
cdecl_type(hb_font_get_glyph_v_kerning_func_t)

cdecl_type(hb_destroy_func_t)
cdecl_type(hb_unicode_combining_class_func_t)
cdecl_type(hb_unicode_general_category_func_t)
cdecl_type(hb_unicode_mirroring_func_t)
cdecl_type(hb_unicode_script_func_t)
cdecl_type(hb_unicode_compose_func_t)
cdecl_type(hb_unicode_decompose_func_t)
cdecl_type(hb_font_get_font_extents_func_t)
cdecl_type(hb_font_get_nominal_glyph_func_t)
cdecl_type(hb_font_get_variation_glyph_func_t)
cdecl_type(hb_font_get_nominal_glyphs_func_t)
cdecl_type(hb_font_get_glyph_advance_func_t)
cdecl_type(hb_font_get_glyph_advances_func_t)
cdecl_type(hb_font_get_glyph_origin_func_t)
cdecl_type(hb_font_get_glyph_kerning_func_t)
cdecl_type(hb_font_get_glyph_extents_func_t)
cdecl_type(hb_font_get_glyph_contour_point_func_t)
cdecl_type(hb_font_get_glyph_name_func_t)
cdecl_type(hb_font_get_glyph_from_name_func_t)
cdecl_type(hb_buffer_message_func_t)
cdecl_type(hb_font_get_glyph_func_t)
cdecl_type(hb_unicode_eastasian_width_func_t)
cdecl_type(hb_unicode_decompose_compatibility_func_t)

cdecl_func(hb_tag_from_string)
cdecl_func(hb_tag_to_string)
cdecl_func(hb_direction_from_string)
cdecl_func(hb_direction_to_string)
cdecl_func(hb_language_from_string)
cdecl_func(hb_language_to_string)
cdecl_func(hb_language_get_default)
cdecl_func(hb_script_from_iso15924_tag)
cdecl_func(hb_script_from_string)
cdecl_func(hb_script_to_iso15924_tag)
cdecl_func(hb_script_get_horizontal_direction)
cdecl_func(hb_feature_from_string)
cdecl_func(hb_feature_to_string)
cdecl_func(hb_variation_from_string)
cdecl_func(hb_variation_to_string)
cdecl_func(hb_color_get_alpha)
cdecl_func(hb_color_get_red)
cdecl_func(hb_color_get_green)
cdecl_func(hb_color_get_blue)
cdecl_func(hb_blob_create)
cdecl_func(hb_blob_create_from_file)
cdecl_func(hb_blob_create_sub_blob)
cdecl_func(hb_blob_copy_writable_or_fail)
cdecl_func(hb_blob_get_empty)
cdecl_func(hb_blob_reference)
cdecl_func(hb_blob_destroy)
cdecl_func(hb_blob_set_user_data)
cdecl_func(hb_blob_get_user_data)
cdecl_func(hb_blob_make_immutable)
cdecl_func(hb_blob_is_immutable)
cdecl_func(hb_blob_get_length)
cdecl_func(hb_blob_get_data)
cdecl_func(hb_blob_get_data_writable)
cdecl_func(hb_unicode_funcs_get_default)
cdecl_func(hb_unicode_funcs_create)
cdecl_func(hb_unicode_funcs_get_empty)
cdecl_func(hb_unicode_funcs_reference)
cdecl_func(hb_unicode_funcs_destroy)
cdecl_func(hb_unicode_funcs_set_user_data)
cdecl_func(hb_unicode_funcs_get_user_data)
cdecl_func(hb_unicode_funcs_make_immutable)
cdecl_func(hb_unicode_funcs_is_immutable)
cdecl_func(hb_unicode_funcs_get_parent)
cdecl_func(hb_unicode_funcs_set_combining_class_func)
cdecl_func(hb_unicode_funcs_set_general_category_func)
cdecl_func(hb_unicode_funcs_set_mirroring_func)
cdecl_func(hb_unicode_funcs_set_script_func)
cdecl_func(hb_unicode_funcs_set_compose_func)
cdecl_func(hb_unicode_funcs_set_decompose_func)
cdecl_func(hb_unicode_combining_class)
cdecl_func(hb_unicode_general_category)
cdecl_func(hb_unicode_mirroring)
cdecl_func(hb_unicode_script)
cdecl_func(hb_unicode_compose)
cdecl_func(hb_unicode_decompose)
cdecl_func(hb_set_create)
cdecl_func(hb_set_get_empty)
cdecl_func(hb_set_reference)
cdecl_func(hb_set_destroy)
cdecl_func(hb_set_set_user_data)
cdecl_func(hb_set_get_user_data)
cdecl_func(hb_set_allocation_successful)
cdecl_func(hb_set_clear)
cdecl_func(hb_set_is_empty)
cdecl_func(hb_set_has)
cdecl_func(hb_set_add)
cdecl_func(hb_set_add_range)
cdecl_func(hb_set_del)
cdecl_func(hb_set_del_range)
cdecl_func(hb_set_is_equal)
cdecl_func(hb_set_is_subset)
cdecl_func(hb_set_set)
cdecl_func(hb_set_union)
cdecl_func(hb_set_intersect)
cdecl_func(hb_set_subtract)
cdecl_func(hb_set_symmetric_difference)
cdecl_func(hb_set_get_population)
cdecl_func(hb_set_get_min)
cdecl_func(hb_set_get_max)
cdecl_func(hb_set_next)
cdecl_func(hb_set_previous)
cdecl_func(hb_set_next_range)
cdecl_func(hb_set_previous_range)
cdecl_func(hb_face_count)
cdecl_func(hb_face_create)
cdecl_func(hb_face_create_for_tables)
cdecl_func(hb_face_get_empty)
cdecl_func(hb_face_reference)
cdecl_func(hb_face_destroy)
cdecl_func(hb_face_set_user_data)
cdecl_func(hb_face_get_user_data)
cdecl_func(hb_face_make_immutable)
cdecl_func(hb_face_is_immutable)
cdecl_func(hb_face_reference_table)
cdecl_func(hb_face_reference_blob)
cdecl_func(hb_face_set_index)
cdecl_func(hb_face_get_index)
cdecl_func(hb_face_set_upem)
cdecl_func(hb_face_get_upem)
cdecl_func(hb_face_set_glyph_count)
cdecl_func(hb_face_get_glyph_count)
cdecl_func(hb_face_get_table_tags)
cdecl_func(hb_face_collect_unicodes)
cdecl_func(hb_face_collect_variation_selectors)
cdecl_func(hb_face_collect_variation_unicodes)
cdecl_func(hb_face_builder_create)
cdecl_func(hb_face_builder_add_table)
cdecl_func(hb_font_funcs_create)
cdecl_func(hb_font_funcs_get_empty)
cdecl_func(hb_font_funcs_reference)
cdecl_func(hb_font_funcs_destroy)
cdecl_func(hb_font_funcs_set_user_data)
cdecl_func(hb_font_funcs_get_user_data)
cdecl_func(hb_font_funcs_make_immutable)
cdecl_func(hb_font_funcs_is_immutable)
cdecl_func(hb_font_funcs_set_font_h_extents_func)
cdecl_func(hb_font_funcs_set_font_v_extents_func)
cdecl_func(hb_font_funcs_set_nominal_glyph_func)
cdecl_func(hb_font_funcs_set_nominal_glyphs_func)
cdecl_func(hb_font_funcs_set_variation_glyph_func)
cdecl_func(hb_font_funcs_set_glyph_h_advance_func)
cdecl_func(hb_font_funcs_set_glyph_v_advance_func)
cdecl_func(hb_font_funcs_set_glyph_h_advances_func)
cdecl_func(hb_font_funcs_set_glyph_v_advances_func)
cdecl_func(hb_font_funcs_set_glyph_h_origin_func)
cdecl_func(hb_font_funcs_set_glyph_v_origin_func)
cdecl_func(hb_font_funcs_set_glyph_h_kerning_func)
cdecl_func(hb_font_funcs_set_glyph_extents_func)
cdecl_func(hb_font_funcs_set_glyph_contour_point_func)
cdecl_func(hb_font_funcs_set_glyph_name_func)
cdecl_func(hb_font_funcs_set_glyph_from_name_func)
cdecl_func(hb_font_get_h_extents)
cdecl_func(hb_font_get_v_extents)
cdecl_func(hb_font_get_nominal_glyph)
cdecl_func(hb_font_get_variation_glyph)
cdecl_func(hb_font_get_nominal_glyphs)
cdecl_func(hb_font_get_glyph_h_advance)
cdecl_func(hb_font_get_glyph_v_advance)
cdecl_func(hb_font_get_glyph_h_advances)
cdecl_func(hb_font_get_glyph_v_advances)
cdecl_func(hb_font_get_glyph_h_origin)
cdecl_func(hb_font_get_glyph_v_origin)
cdecl_func(hb_font_get_glyph_h_kerning)
cdecl_func(hb_font_get_glyph_extents)
cdecl_func(hb_font_get_glyph_contour_point)
cdecl_func(hb_font_get_glyph_name)
cdecl_func(hb_font_get_glyph_from_name)
cdecl_func(hb_font_get_glyph)
cdecl_func(hb_font_get_extents_for_direction)
cdecl_func(hb_font_get_glyph_advance_for_direction)
cdecl_func(hb_font_get_glyph_advances_for_direction)
cdecl_func(hb_font_get_glyph_origin_for_direction)
cdecl_func(hb_font_add_glyph_origin_for_direction)
cdecl_func(hb_font_subtract_glyph_origin_for_direction)
cdecl_func(hb_font_get_glyph_kerning_for_direction)
cdecl_func(hb_font_get_glyph_extents_for_origin)
cdecl_func(hb_font_get_glyph_contour_point_for_origin)
cdecl_func(hb_font_glyph_to_string)
cdecl_func(hb_font_glyph_from_string)
cdecl_func(hb_font_create)
cdecl_func(hb_font_create_sub_font)
cdecl_func(hb_font_get_empty)
cdecl_func(hb_font_reference)
cdecl_func(hb_font_destroy)
cdecl_func(hb_font_set_user_data)
cdecl_func(hb_font_get_user_data)
cdecl_func(hb_font_make_immutable)
cdecl_func(hb_font_is_immutable)
cdecl_func(hb_font_set_parent)
cdecl_func(hb_font_get_parent)
cdecl_func(hb_font_set_face)
cdecl_func(hb_font_get_face)
cdecl_func(hb_font_set_funcs)
cdecl_func(hb_font_set_funcs_data)
cdecl_func(hb_font_set_scale)
cdecl_func(hb_font_get_scale)
cdecl_func(hb_font_set_ppem)
cdecl_func(hb_font_get_ppem)
cdecl_func(hb_font_set_ptem)
cdecl_func(hb_font_get_ptem)
cdecl_func(hb_font_set_variations)
cdecl_func(hb_font_set_var_coords_design)
cdecl_func(hb_font_set_var_coords_normalized)
cdecl_func(hb_font_get_var_coords_normalized)
cdecl_func(hb_font_set_var_named_instance)
cdecl_func(hb_glyph_info_get_glyph_flags)
cdecl_func(hb_segment_properties_equal)
cdecl_func(hb_segment_properties_hash)
cdecl_func(hb_buffer_create)
cdecl_func(hb_buffer_get_empty)
cdecl_func(hb_buffer_reference)
cdecl_func(hb_buffer_destroy)
cdecl_func(hb_buffer_set_user_data)
cdecl_func(hb_buffer_get_user_data)
cdecl_func(hb_buffer_set_content_type)
cdecl_func(hb_buffer_get_content_type)
cdecl_func(hb_buffer_set_unicode_funcs)
cdecl_func(hb_buffer_get_unicode_funcs)
cdecl_func(hb_buffer_set_direction)
cdecl_func(hb_buffer_get_direction)
cdecl_func(hb_buffer_set_script)
cdecl_func(hb_buffer_get_script)
cdecl_func(hb_buffer_set_language)
cdecl_func(hb_buffer_get_language)
cdecl_func(hb_buffer_set_segment_properties)
cdecl_func(hb_buffer_get_segment_properties)
cdecl_func(hb_buffer_guess_segment_properties)
cdecl_func(hb_buffer_set_flags)
cdecl_func(hb_buffer_get_flags)
cdecl_func(hb_buffer_set_cluster_level)
cdecl_func(hb_buffer_get_cluster_level)
cdecl_func(hb_buffer_set_replacement_codepoint)
cdecl_func(hb_buffer_get_replacement_codepoint)
cdecl_func(hb_buffer_set_invisible_glyph)
cdecl_func(hb_buffer_get_invisible_glyph)
cdecl_func(hb_buffer_reset)
cdecl_func(hb_buffer_clear_contents)
cdecl_func(hb_buffer_pre_allocate)
cdecl_func(hb_buffer_allocation_successful)
cdecl_func(hb_buffer_reverse)
cdecl_func(hb_buffer_reverse_range)
cdecl_func(hb_buffer_reverse_clusters)
cdecl_func(hb_buffer_add)
cdecl_func(hb_buffer_add_utf8)
cdecl_func(hb_buffer_add_utf16)
cdecl_func(hb_buffer_add_utf32)
cdecl_func(hb_buffer_add_latin1)
cdecl_func(hb_buffer_add_codepoints)
cdecl_func(hb_buffer_append)
cdecl_func(hb_buffer_set_length)
cdecl_func(hb_buffer_get_length)
cdecl_func(hb_buffer_get_glyph_infos)
cdecl_func(hb_buffer_get_glyph_positions)
cdecl_func(hb_buffer_normalize_glyphs)
cdecl_func(hb_buffer_serialize_format_from_string)
cdecl_func(hb_buffer_serialize_format_to_string)
cdecl_func(hb_buffer_serialize_list_formats)
cdecl_func(hb_buffer_serialize_glyphs)
cdecl_func(hb_buffer_deserialize_glyphs)
cdecl_func(hb_buffer_diff)
cdecl_func(hb_buffer_set_message_func)
//cdecl_func(hb_font_funcs_set_glyph_func)
cdecl_func(hb_set_invert)
//cdecl_func(hb_unicode_funcs_set_eastasian_width_func)
//cdecl_func(hb_unicode_eastasian_width)
//cdecl_func(hb_unicode_funcs_set_decompose_compatibility_func)
//cdecl_func(hb_unicode_decompose_compatibility)
cdecl_func(hb_font_funcs_set_glyph_v_kerning_func)
cdecl_func(hb_font_get_glyph_v_kerning)
cdecl_func(hb_map_create)
cdecl_func(hb_map_get_empty)
cdecl_func(hb_map_reference)
cdecl_func(hb_map_destroy)
cdecl_func(hb_map_set_user_data)
cdecl_func(hb_map_get_user_data)
cdecl_func(hb_map_allocation_successful)
cdecl_func(hb_map_clear)
cdecl_func(hb_map_is_empty)
cdecl_func(hb_map_get_population)
cdecl_func(hb_map_set)
cdecl_func(hb_map_get)
cdecl_func(hb_map_del)
cdecl_func(hb_map_has)
cdecl_func(hb_shape)
cdecl_func(hb_shape_full)
cdecl_func(hb_shape_list_shapers)
cdecl_func(hb_shape_plan_create)
cdecl_func(hb_shape_plan_create_cached)
cdecl_func(hb_shape_plan_create2)
cdecl_func(hb_shape_plan_create_cached2)
cdecl_func(hb_shape_plan_get_empty)
cdecl_func(hb_shape_plan_reference)
cdecl_func(hb_shape_plan_destroy)
cdecl_func(hb_shape_plan_set_user_data)
cdecl_func(hb_shape_plan_get_user_data)
cdecl_func(hb_shape_plan_execute)
cdecl_func(hb_shape_plan_get_shaper)
cdecl_func(hb_version)
cdecl_func(hb_version_string)
cdecl_func(hb_version_atleast)

// LuaJIT is fine with this, makes us independent of require-order
cdecl_type(FT_Face)

cdecl_func(hb_ft_face_create)
cdecl_func(hb_ft_face_create_cached)
cdecl_func(hb_ft_face_create_referenced)
cdecl_func(hb_ft_font_create)
cdecl_func(hb_ft_font_create_referenced)
cdecl_func(hb_ft_font_get_face)
cdecl_func(hb_ft_font_lock_face)
cdecl_func(hb_ft_font_unlock_face)
cdecl_func(hb_ft_font_set_load_flags)
cdecl_func(hb_ft_font_get_load_flags)
cdecl_func(hb_ft_font_changed)
cdecl_func(hb_ft_font_set_funcs)

cdecl_type(hb_ot_math_glyph_part_flags_t)
cdecl_type(hb_ot_var_axis_flags_t)
cdecl_type(hb_ot_name_id_t)

cdecl_struct(hb_ot_name_entry_t)
cdecl_type(hb_ot_name_entry_t)
cdecl_struct(hb_ot_color_layer_t)
cdecl_type(hb_ot_color_layer_t)
cdecl_struct(hb_ot_var_axis_t)
cdecl_type(hb_ot_var_axis_t)
cdecl_struct(hb_ot_math_glyph_variant_t)
cdecl_type(hb_ot_math_glyph_variant_t)
cdecl_struct(hb_ot_math_glyph_part_t)
cdecl_type(hb_ot_math_glyph_part_t)
cdecl_struct(hb_ot_var_axis_info_t)
cdecl_type(hb_ot_var_axis_info_t)

cdecl_type(hb_ot_color_palette_flags_t)
cdecl_type(hb_ot_layout_glyph_class_t)
cdecl_type(hb_ot_layout_baseline_tag_t)
cdecl_type(hb_ot_math_constant_t)
cdecl_type(hb_ot_math_kern_t)
cdecl_type(hb_ot_meta_tag_t)
cdecl_type(hb_ot_metrics_tag_t)

cdecl_func(hb_ot_name_list_names)
cdecl_func(hb_ot_name_get_utf8)
cdecl_func(hb_ot_name_get_utf16)
cdecl_func(hb_ot_name_get_utf32)
cdecl_func(hb_ot_color_has_palettes)
cdecl_func(hb_ot_color_palette_get_count)
cdecl_func(hb_ot_color_palette_get_name_id)
cdecl_func(hb_ot_color_palette_color_get_name_id)
cdecl_func(hb_ot_color_palette_get_flags)
cdecl_func(hb_ot_color_palette_get_colors)
cdecl_func(hb_ot_color_has_layers)
cdecl_func(hb_ot_color_glyph_get_layers)
cdecl_func(hb_ot_color_has_svg)
cdecl_func(hb_ot_color_glyph_reference_svg)
cdecl_func(hb_ot_color_has_png)
cdecl_func(hb_ot_color_glyph_reference_png)
//cdecl_func(hb_ot_layout_table_choose_script)
//cdecl_func(hb_ot_layout_script_find_language)
//cdecl_func(hb_ot_tags_from_script)
//cdecl_func(hb_ot_tag_from_language)
//cdecl_func(hb_ot_var_get_axes)
//cdecl_func(hb_ot_var_find_axis)
cdecl_func(hb_ot_font_set_funcs)
cdecl_func(hb_ot_tags_from_script_and_language)
cdecl_func(hb_ot_tag_to_script)
cdecl_func(hb_ot_tag_to_language)
cdecl_func(hb_ot_tags_to_script_and_language)
cdecl_func(hb_ot_layout_has_glyph_classes)
cdecl_func(hb_ot_layout_get_glyph_class)
cdecl_func(hb_ot_layout_get_glyphs_in_class)
cdecl_func(hb_ot_layout_get_attach_points)
cdecl_func(hb_ot_layout_get_ligature_carets)
cdecl_func(hb_ot_layout_table_get_script_tags)
cdecl_func(hb_ot_layout_table_find_script)
cdecl_func(hb_ot_layout_table_select_script)
cdecl_func(hb_ot_layout_table_get_feature_tags)
cdecl_func(hb_ot_layout_script_get_language_tags)
cdecl_func(hb_ot_layout_script_select_language)
cdecl_func(hb_ot_layout_language_get_required_feature_index)
cdecl_func(hb_ot_layout_language_get_required_feature)
cdecl_func(hb_ot_layout_language_get_feature_indexes)
cdecl_func(hb_ot_layout_language_get_feature_tags)
cdecl_func(hb_ot_layout_language_find_feature)
cdecl_func(hb_ot_layout_feature_get_lookups)
cdecl_func(hb_ot_layout_table_get_lookup_count)
cdecl_func(hb_ot_layout_collect_features)
cdecl_func(hb_ot_layout_collect_lookups)
cdecl_func(hb_ot_layout_lookup_collect_glyphs)
cdecl_func(hb_ot_layout_table_find_feature_variations)
cdecl_func(hb_ot_layout_feature_with_variations_get_lookups)
cdecl_func(hb_ot_layout_has_substitution)
cdecl_func(hb_ot_layout_lookup_get_glyph_alternates)
cdecl_func(hb_ot_layout_lookup_would_substitute)
cdecl_func(hb_ot_layout_lookup_substitute_closure)
cdecl_func(hb_ot_layout_lookups_substitute_closure)
cdecl_func(hb_ot_layout_has_positioning)
cdecl_func(hb_ot_layout_get_size_params)
cdecl_func(hb_ot_layout_feature_get_name_ids)
cdecl_func(hb_ot_layout_feature_get_characters)
cdecl_func(hb_ot_layout_get_baseline)
cdecl_func(hb_ot_math_has_data)
cdecl_func(hb_ot_math_get_constant)
cdecl_func(hb_ot_math_get_glyph_italics_correction)
cdecl_func(hb_ot_math_get_glyph_top_accent_attachment)
cdecl_func(hb_ot_math_is_glyph_extended_shape)
cdecl_func(hb_ot_math_get_glyph_kerning)
cdecl_func(hb_ot_math_get_glyph_variants)
cdecl_func(hb_ot_math_get_min_connector_overlap)
cdecl_func(hb_ot_math_get_glyph_assembly)
cdecl_func(hb_ot_meta_get_entry_tags)
cdecl_func(hb_ot_meta_reference_entry)
cdecl_func(hb_ot_metrics_get_position)
cdecl_func(hb_ot_metrics_get_variation)
cdecl_func(hb_ot_metrics_get_x_variation)
cdecl_func(hb_ot_metrics_get_y_variation)
cdecl_func(hb_ot_shape_glyphs_closure)
cdecl_func(hb_ot_shape_plan_collect_lookups)
cdecl_func(hb_ot_var_has_data)
cdecl_func(hb_ot_var_get_axis_count)
cdecl_func(hb_ot_var_get_axis_infos)
cdecl_func(hb_ot_var_find_axis_info)
cdecl_func(hb_ot_var_get_named_instance_count)
cdecl_func(hb_ot_var_named_instance_get_subfamily_name_id)
cdecl_func(hb_ot_var_named_instance_get_postscript_name_id)
cdecl_func(hb_ot_var_named_instance_get_design_coords)
cdecl_func(hb_ot_var_normalize_variations)
cdecl_func(hb_ot_var_normalize_coords)

// Annoyingly, this one is an anonymous enum in <harfbuzz/hb-ot-name.h>...
cdecl_const(HB_OT_NAME_ID_COPYRIGHT)
cdecl_const(HB_OT_NAME_ID_FONT_FAMILY)
cdecl_const(HB_OT_NAME_ID_FONT_SUBFAMILY)
cdecl_const(HB_OT_NAME_ID_UNIQUE_ID)
cdecl_const(HB_OT_NAME_ID_FULL_NAME)
cdecl_const(HB_OT_NAME_ID_VERSION_STRING)
cdecl_const(HB_OT_NAME_ID_POSTSCRIPT_NAME)
cdecl_const(HB_OT_NAME_ID_TRADEMARK)
cdecl_const(HB_OT_NAME_ID_MANUFACTURER)
cdecl_const(HB_OT_NAME_ID_DESIGNER)
cdecl_const(HB_OT_NAME_ID_DESCRIPTION)
cdecl_const(HB_OT_NAME_ID_VENDOR_URL)
cdecl_const(HB_OT_NAME_ID_DESIGNER_URL)
cdecl_const(HB_OT_NAME_ID_LICENSE)
cdecl_const(HB_OT_NAME_ID_LICENSE_URL)
cdecl_const(HB_OT_NAME_ID_TYPOGRAPHIC_FAMILY)
cdecl_const(HB_OT_NAME_ID_TYPOGRAPHIC_SUBFAMILY)
cdecl_const(HB_OT_NAME_ID_MAC_FULL_NAME)
cdecl_const(HB_OT_NAME_ID_SAMPLE_TEXT)
cdecl_const(HB_OT_NAME_ID_CID_FINDFONT_NAME)
cdecl_const(HB_OT_NAME_ID_WWS_FAMILY)
cdecl_const(HB_OT_NAME_ID_WWS_SUBFAMILY)
cdecl_const(HB_OT_NAME_ID_LIGHT_BACKGROUND)
cdecl_const(HB_OT_NAME_ID_DARK_BACKGROUND)
cdecl_const(HB_OT_NAME_ID_VARIATIONS_PS_PREFIX)

// HB 2.7.3
cdecl_func(hb_buffer_has_positions)
cdecl_func(hb_buffer_serialize)
cdecl_func(hb_buffer_serialize_unicode)
cdecl_func(hb_buffer_deserialize_unicode)

// HB 2.8.2
cdecl_func(hb_blob_create_or_fail)
cdecl_func(hb_blob_create_from_file_or_fail)
cdecl_func(hb_set_copy)

// HB 3.0.0
cdecl_type(hb_style_tag_t)
cdecl_func(hb_style_get_value)
//cdecl_struct(hb_subset_input_t)
//cdecl_enum(hb_subset_flags_t)
//cdecl_enum(hb_subset_sets_t)
//cdecl_func(hb_subset_input_create_or_fail)
//cdecl_func(hb_subset_input_reference)
//cdecl_func(hb_subset_input_destroy)
//cdecl_func(hb_subset_input_set_user_data)
//cdecl_func(hb_subset_input_get_user_data)
//cdecl_func(hb_subset_input_unicode_set)
//cdecl_func(hb_subset_input_glyph_set)
//cdecl_func(hb_subset_input_set)
//cdecl_func(hb_subset_input_get_flags)
//cdecl_func(hb_subset_input_set_flags)
//cdecl_func(hb_subset_or_fail)

// HB 3.1.0
cdecl_func(hb_buffer_set_not_found_glyph)
cdecl_func(hb_buffer_get_not_found_glyph)

// HB 3.3.0
cdecl_func(hb_segment_properties_overlay)
cdecl_func(hb_buffer_create_similar)
cdecl_func(hb_font_set_synthetic_slant)
cdecl_func(hb_font_get_synthetic_slant)
cdecl_func(hb_font_get_var_coords_design)

// HB 3.4.0
cdecl_const(HB_OT_TAG_MATH_SCRIPT)
cdecl_type(hb_ot_math_kern_entry_t)
cdecl_func(hb_ot_math_get_glyph_kernings)

// HB 4.0.0
cdecl_union(_hb_var_num_t)
cdecl_type(hb_var_num_t)
cdecl_struct(hb_draw_state_t)
cdecl_type(hb_draw_state_t)
cdecl_type(hb_draw_funcs_t)
cdecl_func(hb_draw_funcs_create)
cdecl_func(hb_draw_funcs_reference)
cdecl_func(hb_draw_funcs_destroy)
cdecl_func(hb_draw_funcs_is_immutable)
cdecl_func(hb_draw_funcs_make_immutable)
cdecl_type(hb_draw_move_to_func_t)
cdecl_func(hb_draw_funcs_set_move_to_func)
cdecl_type(hb_draw_line_to_func_t)
cdecl_func(hb_draw_funcs_set_line_to_func)
cdecl_type(hb_draw_quadratic_to_func_t)
cdecl_func(hb_draw_funcs_set_quadratic_to_func)
cdecl_type(hb_draw_cubic_to_func_t)
cdecl_func(hb_draw_funcs_set_cubic_to_func)
cdecl_type(hb_draw_close_path_func_t)
cdecl_func(hb_draw_funcs_set_close_path_func)
//cdecl_struct(HB_DRAW_STATE_DEFAULT)
cdecl_func(hb_draw_move_to)
cdecl_func(hb_draw_line_to)
cdecl_func(hb_draw_quadratic_to)
cdecl_func(hb_draw_cubic_to)
cdecl_func(hb_draw_close_path)
cdecl_type(hb_font_get_glyph_shape_func_t)
cdecl_func(hb_font_funcs_set_glyph_shape_func)
cdecl_func(hb_font_get_glyph_shape)
cdecl_func(hb_ot_layout_get_horizontal_baseline_tag_for_script)
cdecl_func(hb_ot_layout_get_baseline_with_fallback)
cdecl_func(hb_ot_metrics_get_position_with_fallback)
//cdecl_struct(hb_subset_plan_t)
//cdecl_func(hb_subset_plan_create_or_fail)
//cdecl_func(hb_subset_plan_reference)
//cdecl_func(hb_subset_plan_destroy)
//cdecl_func(hb_subset_plan_set_user_data)
//cdecl_func(hb_subset_plan_get_user_data)
//cdecl_func(hb_subset_plan_execute_or_fail)
//cdecl_func(hb_subset_plan_unicode_to_old_glyph_mapping)
//cdecl_func(hb_subset_plan_new_to_old_glyph_mapping)
//cdecl_func(hb_subset_plan_old_to_new_glyph_mapping)

// HB 4.1.0
cdecl_func(hb_set_add_sorted_array)

// HB 4.2.0
cdecl_func(hb_set_next_many)

// HB 4.3.0
cdecl_func(hb_map_is_equal)

// HB 4.4.0
cdecl_func(hb_font_changed)
cdecl_func(hb_font_get_serial)
cdecl_func(hb_ft_hb_font_changed)
cdecl_func(hb_set_hash)
cdecl_func(hb_map_copy)
cdecl_func(hb_map_hash)

// HB 5.0.0
cdecl_func(hb_language_matches)

// HB 5.3.0
cdecl_func(hb_ot_layout_lookup_get_optical_bound)
cdecl_func(hb_face_builder_sort_tables)

// HB 6.0.0
cdecl_func(hb_subset_input_pin_axis_location)
cdecl_func(hb_subset_input_pin_axis_to_default)
cdecl_func(hb_subset_preprocess)

// HB 7.0.0
+HB_FONT_NO_VAR_NAMED_INSTANCE
+HB_PAINT_IMAGE_FORMAT_BGRA
+HB_PAINT_IMAGE_FORMAT_PNG
+HB_PAINT_IMAGE_FORMAT_SVG
+hb_cairo_font_face_create_for_face
+hb_cairo_font_face_create_for_font
+hb_cairo_font_face_get_face
+hb_cairo_font_face_get_font
+hb_cairo_font_face_get_scale_factor
+hb_cairo_font_face_set_font_init_func
+hb_cairo_font_face_set_scale_factor
+hb_cairo_font_init_func_t
+hb_cairo_glyphs_from_buffer
+hb_cairo_scaled_font_get_font
+hb_color_line_get_color_stops
+hb_color_line_get_color_stops_func_t
+hb_color_line_get_extend
+hb_color_line_get_extend_func_t
+hb_color_line_t
+hb_color_stop_t
+hb_draw_funcs_get_empty
+hb_draw_funcs_get_user_data
+hb_draw_funcs_set_user_data
+hb_face_collect_nominal_glyph_mapping
+hb_font_draw_glyph
+hb_font_draw_glyph_func_t
+hb_font_funcs_set_draw_glyph_func
+hb_font_funcs_set_paint_glyph_func
+hb_font_get_synthetic_bold
+hb_font_get_var_named_instance
+hb_font_paint_glyph
+hb_font_paint_glyph_func_t
+hb_font_set_synthetic_bold
+hb_map_keys
+hb_map_next
+hb_map_update
+hb_map_values
+hb_ot_color_glyph_has_paint
+hb_ot_color_has_paint
+hb_ot_layout_script_select_language2
+hb_ot_name_id_predefined_t
+hb_paint_color
+hb_paint_color_func_t
+hb_paint_composite_mode_t
+hb_paint_custom_palette_color
+hb_paint_custom_palette_color_func_t
+hb_paint_extend_t
+hb_paint_funcs_create
+hb_paint_funcs_destroy
+hb_paint_funcs_get_empty
+hb_paint_funcs_get_user_data
+hb_paint_funcs_is_immutable
+hb_paint_funcs_make_immutable
+hb_paint_funcs_reference
+hb_paint_funcs_set_color_func
+hb_paint_funcs_set_custom_palette_color_func
+hb_paint_funcs_set_image_func
+hb_paint_funcs_set_linear_gradient_func
+hb_paint_funcs_set_pop_clip_func
+hb_paint_funcs_set_pop_group_func
+hb_paint_funcs_set_pop_transform_func
+hb_paint_funcs_set_push_clip_glyph_func
+hb_paint_funcs_set_push_clip_rectangle_func
+hb_paint_funcs_set_push_group_func
+hb_paint_funcs_set_push_transform_func
+hb_paint_funcs_set_radial_gradient_func
+hb_paint_funcs_set_sweep_gradient_func
+hb_paint_funcs_set_user_data
+hb_paint_funcs_t
+hb_paint_image
+hb_paint_image_func_t
+hb_paint_linear_gradient
+hb_paint_linear_gradient_func_t
+hb_paint_pop_clip
+hb_paint_pop_clip_func_t
+hb_paint_pop_group
+hb_paint_pop_group_func_t
+hb_paint_pop_transform
+hb_paint_pop_transform_func_t
+hb_paint_push_clip_glyph
+hb_paint_push_clip_glyph_func_t
+hb_paint_push_clip_rectangle
+hb_paint_push_clip_rectangle_func_t
+hb_paint_push_group
+hb_paint_push_group_func_t
+hb_paint_push_transform
+hb_paint_push_transform_func_t
+hb_paint_radial_gradient
+hb_paint_radial_gradient_func_t
+hb_paint_sweep_gradient
+hb_paint_sweep_gradient_func_t
+hb_set_is_inverted
+hb_subset_input_keep_everything

// HB 7.1.0
+hb_font_set_variation()

// HB 7.2.0
+HB_SUBSET_FLAGS_NO_LAYOUT_CLOSURE
+HB_UNICODE_COMBINING_CLASS_CCC132

// HB 7.3.0
+hb_subset_input_old_to_new_glyph_mapping()

// HB 8.0.0
+HB_CODEPOINT_INVALID
+hb_ot_layout_get_baseline2()
+hb_ot_layout_get_baseline_with_fallback2()
+hb_ot_layout_get_font_extents()
+hb_ot_layout_get_font_extents2()
+hb_subset_input_set_axis_range()

// HB 8.1.0
+hb_ot_layout_collect_features_map()
*/