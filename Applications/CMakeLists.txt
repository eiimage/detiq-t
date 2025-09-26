cmake_minimum_required(VERSION 3.21)
project(GenericInterfaceApplications LANGUAGES C CXX)

# Qt6 → nécessite C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Active les outils Qt automatiques
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(../GenericInterface ../ImageIn)

# Qt6 uniquement
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
# (Optionnel) Si tu gardes encore quelques vieilles API Qt5 (QRegExp, QTextCodec...) dans ton code Qt6
# find_package(Qt6 OPTIONAL_COMPONENTS Core5Compat)

#add_executable(basicAlgorithmService
#	BasicApplication/BasicAlgorithmService.cpp
#	BasicApplication/BasicAlgorithmService.h
#	BasicApplication/main.cpp
#)
#target_link_libraries(basicAlgorithmService
#	GenericInterface
#	Qt6::Core
#	Qt6::Widgets
#	Qt6::Gui
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
			PRIVATE
			GenericInterface
			Qt6::Core
			Qt6::Widgets
			Qt6::Gui
)
# Si tu as activé Core5Compat ci-dessus :
# target_link_libraries(binarization PRIVATE Qt6::Core5Compat)
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
			PRIVATE
			GenericInterface
			Qt6::Core
			Qt6::Widgets
			Qt6::Gui
)
# target_link_libraries(bitplane PRIVATE Qt6::Core5Compat)
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
			PRIVATE
			GenericInterface
			Qt6::Core
			Qt6::Widgets
			Qt6::Gui
)
# target_link_libraries(componentlabeling PRIVATE Qt6::Core5Compat)
add_executable(dithering
	./Dithering/DitheringService.cpp
	./Dithering/DitheringService.h
	./Dithering/main.cpp
)
target_link_libraries(dithering
			PRIVATE
			GenericInterface
			Qt6::Core
			Qt6::Widgets
			Qt6::Gui
)
# target_link_libraries(dithering PRIVATE Qt6::Core5Compat)
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
			PRIVATE
			GenericInterface
			Qt6::Core
			Qt6::Widgets
			Qt6::Gui
)
# target_link_libraries(filterMe PRIVATE Qt6::Core5Compat)
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
			PRIVATE
			GenericInterface
			Qt6::Core
			Qt6::Widgets
			Qt6::Gui
)
# target_link_libraries(morphomat PRIVATE Qt6::Core5Compat)
