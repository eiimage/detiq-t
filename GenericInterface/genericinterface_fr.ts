<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr_FR">
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="Widgets/AboutDialog.cpp" line="+27"/>
        <source>About</source>
        <translation>À propos</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>This software has been developed at Insa.</source>
        <translation>Ce logiciel a été développé à l&apos;Insa de Rennes.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Main authors:</source>
        <translation>Auteurs principaux :</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Contributors:</source>
        <translation>Contributeurs :</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Compiled on %1 at %2</source>
        <translation>Compilé le %1 à %2</translation>
    </message>
</context>
<context>
    <name>KeyboardShortcut</name>
    <message>
        <location filename="Widgets/KeyboardShortcut.cpp" line="+27"/>
        <source>Hot keys</source>
        <translation>Raccourcis clavier</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Keyboard Shortcut List</source>
        <translation>Liste des Raccourcis Clavier</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Image Window:</source>
        <oldsource>Image Windows:</oldsource>
        <translation>Fenêtre d&apos;Image:</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+9"/>
        <source>Zoom In</source>
        <translation>Agrandir</translation>
    </message>
    <message>
        <location line="-9"/>
        <source>Ctrl &amp; + / Ctrl &amp; Wheel Forward</source>
        <translation>Ctrl &amp; + / Ctrl &amp; Roule Avant</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Zoom Out</source>
        <translation>Réduire</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Ctrl &amp; - / Ctrl &amp; Wheel Backward</source>
        <translation>Ctrl &amp; - / Ctrl &amp; Roule Arrière</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+10"/>
        <source>Save As</source>
        <translation>Enregistrer Sous</translation>
    </message>
    <message>
        <location line="-9"/>
        <source>Copy Image to Clipboard</source>
        <translation>Copier l&apos;Image Presse-papiers</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Histogram Window (magnifier icon pressed):</source>
        <oldsource>Histogram Windows(magnifier icon pressed):</oldsource>
        <translation>Fenêtre d&apos;Histogramme (icône de la loupe enfoncée):</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Left Area Selection</source>
        <translation>Sélection de Zone par Gauche</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Zoom Out One Step</source>
        <translation>Zoom Arrière Une Etape</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Right Click</source>
        <translation>Clic-Droit</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Zoom Out to Origin</source>
        <translation>Dézoomer Complètement</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Ctrl &amp; Right Click</source>
        <translation>Ctrl &amp; Clic-Droit</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="Widgets/ImageWidgets/DoubleImageWindow.cpp" line="+75"/>
        <source>Offset (127)</source>
        <oldsource> Offset (127) </oldsource>
        <translation>Décalage (127)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Scaling</source>
        <oldsource> Scaling </oldsource>
        <translation>Mise à l&apos;échelle</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Crop</source>
        <translation>Rogner</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Copy &amp; crop</source>
        <translation>Copier &amp; rogner</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Selected</source>
        <translation>Selectionné</translation>
    </message>
    <message>
        <location line="+9"/>
        <location line="+15"/>
        <location line="+97"/>
        <source>Color</source>
        <translation>Couleur</translation>
    </message>
    <message>
        <location line="-106"/>
        <source>Hovered</source>
        <translation>Survolé</translation>
    </message>
    <message>
        <location line="+63"/>
        <source>Pixels Grid</source>
        <translation>Grille de pixels</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>The display image is truncated, values outside the range of [0 , 255] have been eliminated

-------------------------------------------</source>
        <translation>L&apos;image affichée a été rogné, les valeurs en dehors de la plage de [0, 255] sont éliminées

-------------------------------------------</translation>
    </message>
    <message>
        <location line="+127"/>
        <source>Offset applied : val_display = val_image + 127

-------------------------------------------</source>
        <translation>Décalage appliqué: val_affichée = val_image + 127

-------------------------------------------</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Scaling applied : val_display = (val_image &lt; 0 || maxValue == 0) ? 0 : val_image * 255 / maxValue

-------------------------------------------</source>
        <translation>Mise à l&apos;échelle appliquée : val_display = (val_image &lt; 0 || maxValue == 0) ? 0 : val_image * 255 / maxValue

-------------------------------------------</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Both Offset and Scaling applied : val_display = (127-minValue) &gt; (maxValue-127) ? val_image * 127 / (- minValue) + 127 : val_image * 128 / maxValue + 127 

-------------------------------------------</source>
        <oldsource>Both Offset and Scaling applied : val_display = (127-minValue) &gt; (maxValue-127) ? val_image * 127 / (- minValue) + 127 : val_image * 128 / maxValue + 127

-------------------------------------------</oldsource>
        <translation>Décalage et mise à l&apos;échelle appliqués : val_display = (127-minValue) &gt; (maxValue-127) ? val_image * 127 / (- minValue) + 127 : val_image * 128 / maxValue + 127

-------------------------------------------</translation>
    </message>
    <message>
        <source>Scaling applied : val_display = (val_image - minValue) * 255 / (maxValue - minValue)

-------------------------------------------</source>
        <translation type="vanished">Mise à l&apos;échelle appliquée: val_affichée = (val_image - minValue) * 255 / (maxValue - minValue)

-------------------------------------------</translation>
    </message>
    <message>
        <source>Both Offset and Scaling applied : val_display = (val_image - minValue) * 127 / (maxValue - minValue) + 127

-------------------------------------------</source>
        <translation type="vanished">Décalage et mise à l&apos;échelle appliqués: val_affichée = (val_image - minValue) * 127 / (maxValue - minValue) + 127

-------------------------------------------</translation>
    </message>
</context>
<context>
    <name>genericinterface::ColumnView</name>
    <message>
        <location filename="Widgets/ImageWidgets/ColumnView.cpp" line="+28"/>
        <source>Column Profile for column number %1</source>
        <translation>Profil colonne pour la colonne numéro %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pixel Value</source>
        <translation>Valeur du pixel</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Line number</source>
        <translation>Numéro de ligne</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="vanished">Ligne</translation>
    </message>
</context>
<context>
    <name>genericinterface::ColumnWindow</name>
    <message>
        <location filename="Widgets/ImageWidgets/ColumnWindow.cpp" line="+28"/>
        <source>Column profile</source>
        <translation>Profil colonne</translation>
    </message>
</context>
<context>
    <name>genericinterface::DoubleImageWindow</name>
    <message>
        <source>Pixels Grid</source>
        <translation type="vanished">Grille de pixels</translation>
    </message>
    <message>
        <source>Crop</source>
        <translation type="vanished">Rogner</translation>
    </message>
    <message>
        <source>Enable / Disable Offset</source>
        <translation type="vanished">Activer / Désactiver Décalage</translation>
    </message>
    <message>
        <source>Copy &amp; crop</source>
        <translation type="vanished">Copier &amp; rogner</translation>
    </message>
    <message>
        <source>Selected</source>
        <translation type="vanished">Selectionné</translation>
    </message>
    <message>
        <location filename="Widgets/ImageWidgets/DoubleImageWindow.cpp" line="-197"/>
        <source>Color</source>
        <translation>Couleur</translation>
    </message>
    <message>
        <source>Hovered</source>
        <translation type="vanished">Survolé</translation>
    </message>
    <message>
        <source>Disable Offset</source>
        <translation type="vanished">Désactiver le décalage des pixels</translation>
    </message>
    <message>
        <source>Offset (127)</source>
        <translation type="vanished">Décalage (127)</translation>
    </message>
</context>
<context>
    <name>genericinterface::FileService</name>
    <message>
        <location filename="Services/FileService.cpp" line="+43"/>
        <location line="+50"/>
        <location line="+5"/>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <location line="-54"/>
        <location line="+31"/>
        <source>&amp;Open</source>
        <translation>&amp;Ouvrir</translation>
    </message>
    <message>
        <location line="-26"/>
        <source>Save &amp;As</source>
        <translation>&amp;Enregistrer sous</translation>
    </message>
    <message>
        <source>Bad object type</source>
        <translation type="vanished">Type d&apos;objet incorrect</translation>
    </message>
    <message>
        <source>Only images can be saved to a file.</source>
        <translation type="vanished">Seules les images peuvent être sauvegardées dans un fichier.</translation>
    </message>
    <message>
        <source>Unknown exception</source>
        <translation type="vanished">Exception inconnue</translation>
    </message>
    <message>
        <location line="+151"/>
        <source>Save a file</source>
        <translation>Sauvegarder dans un fichier</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>Open a file</source>
        <translation>Ouvrir un fichier</translation>
    </message>
    <message>
        <location line="-20"/>
        <source>&amp;%1 %2</source>
        <translation>&amp;%1 %2</translation>
    </message>
    <message>
        <location line="-34"/>
        <source>PNG image (*.png);;BMP image (*.bmp);; JPEG image(*.jpg *.jpeg);; VFF image (*.vff)</source>
        <translation>Image PNG (*.png);;Image BMP (*.bmp);; Image JPEG(*.jpg *.jpeg);; Image VFF (*.vff)</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>Supported image (*.png *.bmp *.jpg *.jpeg *.vff);; PNG image (*.png);;BMP image (*.bmp);; JPEG image(*.jpg *.jpeg);; VFF image (*.vff)</source>
        <translation>Images supportées (*.png *.bmp *.jpg *.jpeg *.vff);; Image PNG (*.png);;Image BMP (*.bmp);; Image JPEG (*.jpg *.jpeg);; VFF image (*.vff)</translation>
    </message>
    <message>
        <location line="-174"/>
        <source>Language</source>
        <translation>Langue</translation>
    </message>
    <message>
        <location line="+193"/>
        <source>Change current language</source>
        <translation>Changer de langue</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The current language will be modified at next startup. Would you like to restart now?</source>
        <translation>La langue sera changée au prochain démarrage. Voulez-vous redémarrer maintenant ?</translation>
    </message>
    <message>
        <location line="-219"/>
        <source>&amp;Copy</source>
        <translation>&amp;Copier</translation>
    </message>
</context>
<context>
    <name>genericinterface::GenericHistogramView</name>
    <message>
        <location filename="Widgets/ImageWidgets/GenericHistogramView.cpp" line="+150"/>
        <source>Histogram</source>
        <translation>Histogramme</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Number of specimen</source>
        <translation>Nombre d&apos;éléments</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pixel value</source>
        <translation>Valeur de pixel</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>Black</source>
        <translation>Noir</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Red</source>
        <translation>Rouge</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+8"/>
        <source>Alpha</source>
        <translation>Alpha</translation>
    </message>
    <message>
        <location line="-6"/>
        <source>Green</source>
        <translation>Vert</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Blue</source>
        <translation>Bleu</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Channel</source>
        <translation>Canal</translation>
    </message>
</context>
<context>
    <name>genericinterface::GenericHistogramWindow</name>
    <message>
        <location filename="Widgets/ImageWidgets/GenericHistogramWindow.cpp" line="+113"/>
        <location line="+173"/>
        <source>Hovered</source>
        <oldsource>Hovered: </oldsource>
        <translation>Survolé</translation>
    </message>
    <message>
        <source>Value 1</source>
        <oldsource>Value 1: </oldsource>
        <translation type="vanished">Valeur 1</translation>
    </message>
    <message>
        <source>Value 2</source>
        <oldsource>Value 2: </oldsource>
        <translation type="vanished">Valeur 2</translation>
    </message>
    <message>
        <location line="-223"/>
        <source>Zoom +/-</source>
        <translation>Zoom +/-</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Save As Image</source>
        <translation>Enregistrer l&apos;image sous</translation>
    </message>
    <message>
        <location line="+48"/>
        <location line="+173"/>
        <source>L click value</source>
        <translation>G clic valeur</translation>
    </message>
    <message>
        <location line="-167"/>
        <location line="+173"/>
        <source>R click value</source>
        <translation>D clic valeur</translation>
    </message>
    <message>
        <location line="-138"/>
        <source>Bin size</source>
        <translation>Longueur de l&apos;intervalle</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Enter</source>
        <translation>Entrer</translation>
    </message>
    <message>
        <location line="+47"/>
        <source>Only images can be saved to a file.</source>
        <translation>Seules les images peuvent être sauvegardées dans un fichier.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Bad object type</source>
        <translation>Type d&apos;objet incorrect</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Unknown exception</source>
        <translation>Exception inconnue</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Save a file</source>
        <translation>Sauvegarder dans un fichier</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>PNG image (*.png);;BMP image (*.bmp);; JPEG image(*.jpg *.jpeg);; VFF image (*.vff)</source>
        <translation>Image PNG (*.png);;Image BMP (*.bmp);; Image JPEG(*.jpg *.jpeg);; Image VFF (*.vff)</translation>
    </message>
    <message>
        <location line="+67"/>
        <source>C: %1</source>
        <translation>C: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>C: %1, A: %2</source>
        <translation>C: %1, A: %2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>R: %1, G: %2, B: %3</source>
        <translation>R: %1, V: %2, B: %3</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>R: %1, G: %2, B: %3, A: %4</source>
        <translation>R: %1, V: %2, B: %3, A: %4</translation>
    </message>
</context>
<context>
    <name>genericinterface::GenericInterface</name>
    <message>
        <location filename="GenericInterface.cpp" line="+58"/>
        <source>&amp;Hot keys</source>
        <translation>&amp;Raccourcis clavier</translation>
    </message>
    <message>
        <location line="+32"/>
        <location line="+100"/>
        <source>Interface&apos;s integrity compromised</source>
        <translation>Intégrité de l&apos;interface compromise</translation>
    </message>
    <message>
        <location line="-99"/>
        <source>An error occurred in the addition of a new service. The interface may not offer all the expected features. See the log file for more informations.</source>
        <translation>Une erreur est survenue lors de l&apos;ajout d&apos;un service. Il se peut que l&apos;interface n&apos;offre pas toutes les fonctionnalités attendues.</translation>
    </message>
    <message>
        <location line="+100"/>
        <source>An error occurred in the addition of a service. The interface may not offer all the expected features. See the log file for more informations.</source>
        <translation>Une erreur est survenue lors de l&apos;ajout d&apos;un nouveau service. Il se peut que l&apos;interface n&apos;offre pas toutes les fonctionnalités attendues.</translation>
    </message>
    <message>
        <location line="+44"/>
        <location line="+1"/>
        <source>&amp;Window</source>
        <translation>Fe&amp;nêtre</translation>
    </message>
    <message>
        <location line="+49"/>
        <location line="+1"/>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>&amp;Exit</source>
        <translation>&amp;Quitter</translation>
    </message>
    <message>
        <location line="-229"/>
        <location line="+1"/>
        <location line="+180"/>
        <location line="+1"/>
        <source>&amp;Help</source>
        <translation>&amp;Aide</translation>
    </message>
    <message>
        <location line="-182"/>
        <source>&amp;About</source>
        <translation>&amp;À propos</translation>
    </message>
</context>
<context>
    <name>genericinterface::HistogramView</name>
    <message>
        <location filename="Widgets/ImageWidgets/HistogramView.cpp" line="+30"/>
        <location line="+11"/>
        <location line="+11"/>
        <source>Histogram</source>
        <translation>Histogramme</translation>
    </message>
    <message>
        <location line="-19"/>
        <source>Cumulated histogram</source>
        <translation>Histogramme cumulé</translation>
    </message>
    <message>
        <location line="+11"/>
        <location line="+11"/>
        <source>Cumulative histogram</source>
        <translation>Histogramme cumulé</translation>
    </message>
</context>
<context>
    <name>genericinterface::HistogramWindow</name>
    <message>
        <location filename="Widgets/ImageWidgets/HistogramWindow.cpp" line="+29"/>
        <location line="+11"/>
        <location line="+11"/>
        <source>Histogram</source>
        <translation>Histogramme</translation>
    </message>
    <message>
        <location line="-19"/>
        <location line="+11"/>
        <location line="+11"/>
        <source>Cumulative histogram</source>
        <oldsource>Cumulated histogram</oldsource>
        <translation>Histogramme cumulé</translation>
    </message>
</context>
<context>
    <name>genericinterface::ImageWindow</name>
    <message>
        <location filename="Widgets/ImageWidgets/ImageWindow.cpp" line="+132"/>
        <source>Image</source>
        <translation>Image</translation>
    </message>
    <message>
        <location line="-45"/>
        <location line="+100"/>
        <source>Save As</source>
        <translation>Enregistrer sous</translation>
    </message>
    <message>
        <location line="-44"/>
        <location line="+279"/>
        <source>Zoom</source>
        <translation>Zoom</translation>
    </message>
    <message>
        <location line="-325"/>
        <source>Copy image</source>
        <translation>Copier l&apos;image</translation>
    </message>
    <message>
        <location line="+52"/>
        <source>Selection mode</source>
        <translation>Mode sélection</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Hand mode</source>
        <translation>Mode main</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Select all</source>
        <translation>Sélectionner tout</translation>
    </message>
    <message>
        <location line="-74"/>
        <source>Zoom + (Ctrl+Scroll Wheel)</source>
        <translation>Zoom + (Ctrl+Molette)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Zoom - (Ctrl+Scroll Wheel)</source>
        <translation>Zoom - (Ctrl+Molette)</translation>
    </message>
    <message>
        <location line="+81"/>
        <source>Zoom +</source>
        <translation>Zoom +</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Zoom -</source>
        <translation>Zoom -</translation>
    </message>
    <message>
        <location line="+149"/>
        <source>Only images can be saved to a file.</source>
        <translation>Seules les images peuvent être sauvegardées dans un fichier.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Bad object type</source>
        <translation>Type d&apos;objet incorrect</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Unknown exception</source>
        <translation>Exception inconnue</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Save a file</source>
        <translation>Sauvegarder dans un fichier</translation>
    </message>
    <message>
        <source>PNG image (*.png);;BMP image (*.bmp);; JPEG image(*.jpg *.jpeg);; VFF image (*.vff)</source>
        <translation type="vanished">Image PNG (*.png);;Image BMP (*.bmp);; Image JPEG(*.jpg *.jpeg);; Image VFF (*.vff)</translation>
    </message>
    <message>
        <location line="-257"/>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
    <message>
        <location line="+257"/>
        <source>PNG image (*.png);;BMP image (*.bmp);; JPEG image (*.jpeg *.jpg);; VFF image (*.vff)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+102"/>
        <source>Rename image</source>
        <translation>Renommer l&apos;image</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>New name:</source>
        <translation>Nouveau nom :</translation>
    </message>
    <message>
        <location line="-358"/>
        <source>Apply mask</source>
        <translation>Appliquer un masque</translation>
    </message>
</context>
<context>
    <name>genericinterface::NavigationDock</name>
    <message>
        <location filename="Widgets/NavBar/NavigationDock.cpp" line="+45"/>
        <source>Close all image from the selection</source>
        <translation>Fermer toutes les images de la selection</translation>
    </message>
</context>
<context>
    <name>genericinterface::ProjectionHistogramView</name>
    <message>
        <location filename="Widgets/ImageWidgets/ProjectionHistogramView.cpp" line="+29"/>
        <source>Horizontal Projection Histogram</source>
        <translation>Histogramme de Projection Horizontale</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Vertical Projection Histogram</source>
        <translation>Histogramme de Projection Verticale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Column number</source>
        <translation>Numéro de colonne</translation>
    </message>
    <message>
        <location line="-4"/>
        <source>Line number</source>
        <translation>Numéro de ligne</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Number of pixels &gt;= %1</source>
        <translation>Nombre de pixels &gt;= %1</translation>
    </message>
</context>
<context>
    <name>genericinterface::ProjectionHistogramWindow</name>
    <message>
        <location filename="Widgets/ImageWidgets/ProjectionHistogramWindow.cpp" line="+29"/>
        <source>Horizontal Projection Histogram</source>
        <translation>Histogramme de projection horizontale</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Vertical Projection Histogram</source>
        <translation>Histogramme de projection verticale</translation>
    </message>
</context>
<context>
    <name>genericinterface::RadioPanel</name>
    <message>
        <location filename="Widgets/ImageWidgets/GridView.cpp" line="+60"/>
        <source>R</source>
        <translation>R</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>G</source>
        <translation>V</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>B</source>
        <translation>B</translation>
    </message>
</context>
<context>
    <name>genericinterface::RowView</name>
    <message>
        <location filename="Widgets/ImageWidgets/RowView.cpp" line="+28"/>
        <source>Line Profile for line number %1</source>
        <translation>Profil ligne pour la ligne numéro %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Pixel Value</source>
        <translation>Valeur du pixel</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Column number</source>
        <translation>Numéro de colonne</translation>
    </message>
    <message>
        <source>Column</source>
        <translation type="vanished">Colonne</translation>
    </message>
</context>
<context>
    <name>genericinterface::RowWindow</name>
    <message>
        <location filename="Widgets/ImageWidgets/RowWindow.cpp" line="+28"/>
        <source>Row profile</source>
        <translation>Profil ligne</translation>
    </message>
    <message>
        <source>Column profile</source>
        <translation type="obsolete">Profil colonne</translation>
    </message>
</context>
<context>
    <name>genericinterface::SelectionWidget</name>
    <message>
        <location filename="Widgets/ImageWidgets/ImageWindow.cpp" line="-51"/>
        <source>Selection : </source>
        <translation>Sélection : </translation>
    </message>
    <message>
        <location line="+1"/>
        <source>x:</source>
        <translation>x :</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>y:</source>
        <translation>y : </translation>
    </message>
    <message>
        <location line="+2"/>
        <source>width:</source>
        <translation>largeur :</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>height:</source>
        <translation>hauteur :</translation>
    </message>
</context>
<context>
    <name>genericinterface::StandardImageWindow</name>
    <message>
        <location filename="Widgets/ImageWidgets/StandardImageWindow.cpp" line="+49"/>
        <source>Unknown file format !</source>
        <translation>Format de fichier inconnu !</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Unknown exception</source>
        <translation>Exception inconnue</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Error while opening file</source>
        <translation>Erreur lors de l&apos;ouverture d&apos;un fichier</translation>
    </message>
    <message>
        <location line="+180"/>
        <source>Pixels Grid</source>
        <translation>Grille de pixels</translation>
    </message>
    <message>
        <source>Column Profile</source>
        <translation type="vanished">Profil colonne</translation>
    </message>
    <message>
        <source>Line Profile</source>
        <translation type="vanished">Profil ligne</translation>
    </message>
    <message>
        <location line="-141"/>
        <source>Crop</source>
        <translation>Rogner</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Copy and crop</source>
        <oldsource>Copy &amp; crop</oldsource>
        <translation>Copier et rogner</translation>
    </message>
    <message>
        <location line="+91"/>
        <location line="+12"/>
        <source>Threshold selection</source>
        <translation>Selection du seuil</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Line selection</source>
        <translation>Selection de la ligne</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Select the line to project</source>
        <translation>Selectionnez la ligne à projeter</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Column selection</source>
        <translation>Selection de la colonne</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Select the column to project</source>
        <translation>Selectionnez la colonne à projeter</translation>
    </message>
    <message>
        <location line="-37"/>
        <location line="+12"/>
        <source>Enter the threshold value
(only pixels superior or equal to this value will be counted)</source>
        <translation>Entrez la valeur du seuil
(seuls les pixels supérieurs ou égaux à cette valeur seront comptés)</translation>
    </message>
    <message>
        <location line="-74"/>
        <source>Selected</source>
        <translation>Selectionné</translation>
    </message>
    <message>
        <location line="+9"/>
        <location line="+15"/>
        <location line="+125"/>
        <location line="+15"/>
        <source>Color</source>
        <translation>Couleur</translation>
    </message>
    <message>
        <location line="-149"/>
        <source>Hovered</source>
        <translation>Survolé</translation>
    </message>
    <message>
        <source>Convert to grayscale</source>
        <translation type="vanished">Convertir en niveaux de gris</translation>
    </message>
    <message>
        <source>Convert to binary</source>
        <translation type="vanished">Convertir en binaire</translation>
    </message>
    <message>
        <source>Cumulated histogram</source>
        <translation type="vanished">Histogramme cumulé</translation>
    </message>
</context>
<context>
    <name>genericinterface::UtilityService</name>
    <message>
        <location filename="Services/UtilityService.cpp" line="+30"/>
        <source>&amp;Show</source>
        <translation>&amp;Afficher</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Histogram</source>
        <translation>Histogramme</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Cumulated Histogram</source>
        <translation>Histogramme cumulé</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Horizontal projection histogram</source>
        <translation>Histogramme de projection horizontale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Vertical projection histogram</source>
        <translation>Histogramme de projection verticale</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Line Profile</source>
        <translation>Profil ligne</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Column Profile</source>
        <translation>Profil colonne</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Pixels grid</source>
        <translation>Grille de pixels</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Colors</source>
        <translation>Couleurs</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Convert to Binary</source>
        <translation>Convertir en binaire</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Convert to Grayscale</source>
        <translation>Convertir en niveaux de gris</translation>
    </message>
</context>
<context>
    <name>genericinterface::WindowService</name>
    <message>
        <location filename="Services/WindowService.cpp" line="+43"/>
        <source>Images</source>
        <translation>Images</translation>
    </message>
    <message>
        <location line="+3"/>
        <location line="+6"/>
        <location line="+6"/>
        <source>&amp;Window</source>
        <translation>Fe&amp;nêtre</translation>
    </message>
    <message>
        <location line="-12"/>
        <source>&amp;Tile windows</source>
        <translation>Arranger en &amp;mosaïque</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>&amp;Cascade windows</source>
        <translation>Arranger en &amp;cascade</translation>
    </message>
    <message>
        <location line="+301"/>
        <source>Attention</source>
        <translation>Attention</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>You&apos;re going to close all the relative windows, are you sure you want to continue ?</source>
        <translation>Vous êtes sur le point de fermer toutes les fenêtres relatives, êtes-vous sûr de vouloir continuer ?</translation>
    </message>
    <message>
        <location line="+152"/>
        <source>Mask to apply : </source>
        <translation>Masque à appliquer : </translation>
    </message>
    <message>
        <location line="+64"/>
        <source> - masked</source>
        <translation> - masquée</translation>
    </message>
    <message>
        <location line="-511"/>
        <source>Hide/Show Image dock</source>
        <translation>Cacher/Montrer la barre d&apos;image</translation>
    </message>
</context>
</TS>
