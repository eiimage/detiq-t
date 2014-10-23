project(GenericInterfaceApplications C CXX)
cmake_minimum_required(VERSION 2.8.11)

include_directories(../GenericInterface ../ImageIn)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

#add_executable(basicAlgorithmService
#	BasicApplication/BasicAlgorithmService.cpp
#	BasicApplication/BasicAlgorithmService.h
#	BasicApplication/main.cpp
#)
#target_link_libraries(basicAlgorithmService
#	GenericInterface
#	Qt5::Core
#	Qt5::Widgets
#	Qt5::Gui
#)

add_executable(binarization
	Binarization/Services/BasicTools.cpp
	Binarization/Services/BasicTools.h
	Binarization/Services/BinarizationService.cpp
	Binarization/Services/BinarizationService.h
	Binarization/Widgets/BinarizationWidget.cpp
	Binarization/Widgets/BinarizationWidget.h
	Binarization/main.cpp
)
target_link_libraries(binarization
	GenericInterface
#	Qt5::Core
#	Qt5::Widgets
#	Qt5::Gui
)

add_executable(bitplane
	./BitPlane/main.cpp
	./BitPlane/Services/BitPlaneService.cpp
	./BitPlane/Services/BitPlaneService.h
	./BitPlane/Widgets/BitPlaneChoice.cpp
	./BitPlane/Widgets/BitPlaneChoice.h
	./BitPlane/Widgets/BitPlaneWindow.cpp
	./BitPlane/Widgets/BitPlaneWindow.h
)
target_link_libraries(bitplane
	GenericInterface
#	Qt5::Core
#	Qt5::Widgets
#	Qt5::Gui
)

add_executable(componentlabeling
	./ComponentLabeling/ComponentLabelingService.cpp
	./ComponentLabeling/ComponentLabelingService.h
	./ComponentLabeling/main.cpp
	./ComponentLabeling/OptionsWidget.cpp
	./ComponentLabeling/OptionsWidget.h
	./ComponentLabeling/ResultWidget.cpp
	./ComponentLabeling/ResultWidget.h
)
target_link_libraries(componentlabeling
	GenericInterface
#	Qt5::Core
#	Qt5::Widgets
#	Qt5::Gui
)

add_executable(dithering
	./Dithering/DitheringService.cpp
	./Dithering/DitheringService.h
	./Dithering/main.cpp
)
target_link_libraries(dithering
	GenericInterface
#	Qt5::Core
#	Qt5::Widgets
#	Qt5::Gui
)

add_executable(filterMe
	./FiltrMe/FilteringService.cpp
	./FiltrMe/FilteringService.h
	./FiltrMe/main.cpp
	./FiltrMe/Widgets/FilterChoice.cpp
	./FiltrMe/Widgets/FilterChoice.h
	./FiltrMe/Widgets/FilterEditor.cpp
	./FiltrMe/Widgets/FilterEditor.h
	./FiltrMe/Widgets/FilterEditorItem.cpp
	./FiltrMe/Widgets/FilterEditorItem.h
)
target_link_libraries(filterMe
	GenericInterface
#	Qt5::Core
#	Qt5::Widgets
#	Qt5::Gui
)

add_executable(morphomat
	./MorphoMat/main.cpp
	./MorphoMat/Services/MorphoMatService.cpp
	./MorphoMat/Services/MorphoMatService.h
	./MorphoMat/Widgets/StructElemViewer.cpp
	./MorphoMat/Widgets/StructElemViewer.h
	./MorphoMat/Widgets/StructElemWindow.cpp
	./MorphoMat/Widgets/StructElemWindow.h
)
target_link_libraries(morphomat
	GenericInterface
#	Qt5::Core
#	Qt5::Widgets
#	Qt5::Gui
)

