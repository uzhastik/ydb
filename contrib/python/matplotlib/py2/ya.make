PY2_LIBRARY()

LICENSE(PSF-2.0)

VERSION(2.2.5)

PEERDIR(
    contrib/deprecated/python/backports.functools-lru-cache
    contrib/deprecated/python/functools32
    contrib/python/mock
    contrib/deprecated/python/subprocess32
    contrib/python/cycler
    contrib/python/python-dateutil
    contrib/python/kiwisolver
    contrib/python/matplotlib/py2/src
    contrib/python/matplotlib/py2/matplotlib/tri
    contrib/python/numpy
    contrib/python/pyparsing
    contrib/python/pytz
    contrib/python/six
)

NO_CHECK_IMPORTS(
    matplotlib.backends.*
    matplotlib.sphinxext.*
    matplotlib.testing.*
    mpl_toolkits.*
)

NO_COMPILER_WARNINGS()

NO_LINT()

FROM_SANDBOX(FILE 1119073729 OUT_NOAUTO mpl-data.tar.gz)

RESOURCE(
    mpl-data.tar.gz /mpl-data/mpl-data.tar.gz
)

PY_SRCS(
    TOP_LEVEL
    matplotlib/__init__.py
    matplotlib/_animation_data.py
    matplotlib/_cm.py
    matplotlib/_cm_listed.py
    matplotlib/_color_data.py
    matplotlib/_constrained_layout.py
    matplotlib/_layoutbox.py
    matplotlib/_mathtext_data.py
    matplotlib/_pylab_helpers.py
    matplotlib/_version.py
    matplotlib/afm.py
    matplotlib/animation.py
    matplotlib/artist.py
    matplotlib/axes/__init__.py
    matplotlib/axes/_axes.py
    matplotlib/axes/_base.py
    matplotlib/axes/_subplots.py
    matplotlib/axis.py
    matplotlib/backend_bases.py
    matplotlib/backend_managers.py
    matplotlib/backend_tools.py
    matplotlib/backends/__init__.py
    matplotlib/backends/_backend_tk.py
    matplotlib/backends/_gtk3_compat.py
    matplotlib/backends/backend_agg.py
    matplotlib/backends/backend_cairo.py
    matplotlib/backends/backend_gdk.py
    matplotlib/backends/backend_gtk.py
    matplotlib/backends/backend_gtk3.py
    matplotlib/backends/backend_gtk3agg.py
    matplotlib/backends/backend_gtk3cairo.py
    matplotlib/backends/backend_gtkagg.py
    matplotlib/backends/backend_gtkcairo.py
    matplotlib/backends/backend_macosx.py
    matplotlib/backends/backend_mixed.py
    matplotlib/backends/backend_nbagg.py
    matplotlib/backends/backend_pdf.py
    matplotlib/backends/backend_pgf.py
    matplotlib/backends/backend_ps.py
    matplotlib/backends/backend_qt4.py
    matplotlib/backends/backend_qt4agg.py
    matplotlib/backends/backend_qt4cairo.py
    matplotlib/backends/backend_qt5.py
    matplotlib/backends/backend_qt5agg.py
    matplotlib/backends/backend_qt5cairo.py
    matplotlib/backends/backend_svg.py
    matplotlib/backends/backend_template.py
    matplotlib/backends/backend_tkagg.py
    matplotlib/backends/backend_tkcairo.py
    matplotlib/backends/backend_webagg.py
    matplotlib/backends/backend_webagg_core.py
    matplotlib/backends/backend_wx.py
    matplotlib/backends/backend_wxagg.py
    matplotlib/backends/backend_wxcairo.py
    matplotlib/backends/qt_compat.py
    matplotlib/backends/qt_editor/__init__.py
    matplotlib/backends/qt_editor/figureoptions.py
    matplotlib/backends/qt_editor/formlayout.py
    matplotlib/backends/qt_editor/formsubplottool.py
    matplotlib/backends/tkagg.py
    matplotlib/backends/windowing.py
    matplotlib/backends/wx_compat.py
    matplotlib/bezier.py
    matplotlib/blocking_input.py
    matplotlib/category.py
    matplotlib/cbook/__init__.py
    matplotlib/cbook/_backports.py
    matplotlib/cbook/deprecation.py
    matplotlib/cm.py
    matplotlib/collections.py
    matplotlib/colorbar.py
    matplotlib/colors.py
    matplotlib/compat/__init__.py
    matplotlib/compat/subprocess.py
    matplotlib/container.py
    matplotlib/contour.py
    matplotlib/dates.py
    matplotlib/docstring.py
    matplotlib/dviread.py
    matplotlib/figure.py
    matplotlib/font_manager.py
    matplotlib/fontconfig_pattern.py
    matplotlib/gridspec.py
    matplotlib/hatch.py
    matplotlib/image.py
    matplotlib/legend.py
    matplotlib/legend_handler.py
    matplotlib/lines.py
    matplotlib/markers.py
    matplotlib/mathtext.py
    matplotlib/mlab.py
    matplotlib/offsetbox.py
    matplotlib/patches.py
    matplotlib/path.py
    matplotlib/patheffects.py
    matplotlib/projections/__init__.py
    matplotlib/projections/geo.py
    matplotlib/projections/polar.py
    matplotlib/pylab.py
    matplotlib/pyplot.py
    matplotlib/quiver.py
    matplotlib/rcsetup.py
    matplotlib/sankey.py
    matplotlib/scale.py
    matplotlib/sphinxext/__init__.py
    matplotlib/sphinxext/mathmpl.py
    matplotlib/sphinxext/only_directives.py
    matplotlib/sphinxext/plot_directive.py
    matplotlib/spines.py
    matplotlib/stackplot.py
    matplotlib/streamplot.py
    matplotlib/style/__init__.py
    matplotlib/style/core.py
    matplotlib/table.py
    matplotlib/testing/__init__.py
    matplotlib/testing/_nose/__init__.py
    matplotlib/testing/_nose/decorators.py
    matplotlib/testing/_nose/exceptions.py
    matplotlib/testing/_nose/plugins/__init__.py
    matplotlib/testing/_nose/plugins/knownfailure.py
    matplotlib/testing/_nose/plugins/performgc.py
    matplotlib/testing/compare.py
    matplotlib/testing/conftest.py
    matplotlib/testing/decorators.py
    matplotlib/testing/determinism.py
    matplotlib/testing/disable_internet.py
    matplotlib/testing/exceptions.py
    matplotlib/testing/jpl_units/Duration.py
    matplotlib/testing/jpl_units/Epoch.py
    matplotlib/testing/jpl_units/EpochConverter.py
    matplotlib/testing/jpl_units/StrConverter.py
    matplotlib/testing/jpl_units/UnitDbl.py
    matplotlib/testing/jpl_units/UnitDblConverter.py
    matplotlib/testing/jpl_units/UnitDblFormatter.py
    matplotlib/testing/jpl_units/__init__.py
    matplotlib/testing/noseclasses.py
    matplotlib/texmanager.py
    matplotlib/text.py
    matplotlib/textpath.py
    matplotlib/ticker.py
    matplotlib/tight_bbox.py
    matplotlib/tight_layout.py
    matplotlib/transforms.py
    matplotlib/type1font.py
    matplotlib/units.py
    matplotlib/widgets.py
    mpl_toolkits/__init__.py
    mpl_toolkits/axes_grid/__init__.py
    mpl_toolkits/axes_grid/anchored_artists.py
    mpl_toolkits/axes_grid/angle_helper.py
    mpl_toolkits/axes_grid/axes_divider.py
    mpl_toolkits/axes_grid/axes_grid.py
    mpl_toolkits/axes_grid/axes_rgb.py
    mpl_toolkits/axes_grid/axes_size.py
    mpl_toolkits/axes_grid/axis_artist.py
    mpl_toolkits/axes_grid/axisline_style.py
    mpl_toolkits/axes_grid/axislines.py
    mpl_toolkits/axes_grid/clip_path.py
    mpl_toolkits/axes_grid/colorbar.py
    mpl_toolkits/axes_grid/floating_axes.py
    mpl_toolkits/axes_grid/grid_finder.py
    mpl_toolkits/axes_grid/grid_helper_curvelinear.py
    mpl_toolkits/axes_grid/inset_locator.py
    mpl_toolkits/axes_grid/parasite_axes.py
    mpl_toolkits/axes_grid1/__init__.py
    mpl_toolkits/axes_grid1/anchored_artists.py
    mpl_toolkits/axes_grid1/axes_divider.py
    mpl_toolkits/axes_grid1/axes_grid.py
    mpl_toolkits/axes_grid1/axes_rgb.py
    mpl_toolkits/axes_grid1/axes_size.py
    mpl_toolkits/axes_grid1/colorbar.py
    mpl_toolkits/axes_grid1/inset_locator.py
    mpl_toolkits/axes_grid1/mpl_axes.py
    mpl_toolkits/axes_grid1/parasite_axes.py
    mpl_toolkits/axisartist/__init__.py
    mpl_toolkits/axisartist/angle_helper.py
    mpl_toolkits/axisartist/axes_divider.py
    mpl_toolkits/axisartist/axes_grid.py
    mpl_toolkits/axisartist/axes_rgb.py
    mpl_toolkits/axisartist/axis_artist.py
    mpl_toolkits/axisartist/axisline_style.py
    mpl_toolkits/axisartist/axislines.py
    mpl_toolkits/axisartist/clip_path.py
    mpl_toolkits/axisartist/floating_axes.py
    mpl_toolkits/axisartist/grid_finder.py
    mpl_toolkits/axisartist/grid_helper_curvelinear.py
    mpl_toolkits/axisartist/parasite_axes.py
    mpl_toolkits/mplot3d/__init__.py
    mpl_toolkits/mplot3d/art3d.py
    mpl_toolkits/mplot3d/axes3d.py
    mpl_toolkits/mplot3d/axis3d.py
    mpl_toolkits/mplot3d/proj3d.py
    pylab.py
)

END()

RECURSE(
    extern
    matplotlib/tri
    src
)
