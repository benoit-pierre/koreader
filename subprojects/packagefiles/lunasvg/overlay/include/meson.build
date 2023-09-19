cxx = meson.get_compiler('cpp')

cdata = configuration_data()

if cxx.has_function_attribute('deprecated')
  cdata.set('LUNASVG_DEPRECATED', '__attribute__((__deprecated__))')
endif

if cxx.has_function_attribute('visibility:default')
  cdata.set('LUNASVG_EXPORT', '__attribute__((visibility("default")))')
endif

if cxx.has_function_attribute('visibility:hidden')
  cdata.set('LUNASVG_NO_EXPORT', '__attribute__((visibility("hidden")))')
endif

lunasvg_export_h = configure_file(
  configuration: cdata,
  input: 'lunasvg_export.meson.h',
  output: 'lunasvg_export.h',
)
