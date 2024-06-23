
add_requires("sol2","qt5widgets")
set_languages("c++17")
add_rules("mode.debug", "mode.release")


target("autolua_jinlan")
  add_rules("qt.widgetapp")
  set_kind("binary")
  add_files("src/*.cc")
    add_headerfiles("src/*.h")
  add_packages("sol2","qt5widgets")
  if is_plat("linux") then
    add_cxflags("-fPIC")
  end
  add_files("src/FunctionList.h",
    "src/ImageWidget.h",
    "src/MagnifierView.h",
    "src/MainWindow.h",
    "src/PointItem.h",
    "src/ToolbarWidget.h",
    "resources/*.qrc")