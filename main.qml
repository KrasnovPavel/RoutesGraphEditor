import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Routes Graph Editor")

    FileDialog {
        id: openImageDialog

        title: "Please choose an image"
        folder: shortcuts.home
        nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]

        onAccepted: mainImage.source = fileDialog.fileUrl
    }

    FileDialog {
        id: openXmlDialog

        title: "Please choose a xml file"
        folder: shortcuts.home
        nameFilters: [ "XML files (*.xml)", "All files (*)" ]

        onAccepted: xmlHandler.openXML(openXmlDialog.fileUrl)
    }

    RowLayout {
        id: buttonsRow

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Button {
            text: "Открыть изображение..."

            onClicked: openImageDialog.open()
        }

        Button {
            text: "Выбрать xml..."

            onClicked: openXmlDialog.open()
        }

        Button {
            text: "Очистить метки"

            onClicked: {
                dotsModel.clear();
                linesModel.clear();
            }
        }

        Button {
            text: "Сохранить"

            onClicked: xmlHandler.saveXML()
        }
    }

    ScrollView {
        anchors {
            top: buttonsRow.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        clip: true

        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOn

        Image {
            id: mainImage

            source: "qrc:/images/map.png"

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                onClicked: {
                    if (!lineView.isConstructingLine) {
                        if (mouse.button === Qt.LeftButton) {
                            dotsModel.append(dotsModel.getNewID(), mouseX, mouseY);
                        }
                    } else {
                        if (mouse.button === Qt.RightButton) {
                            linesModel.remove(linesModel.count - 1);
                        }
                    }
                }

                onMouseXChanged: {
                    if (lineView.isConstructingLine) {
                        lineView.itemAt(lineView.count - 1).followMouse(mouseX, mouseY);
                    }
                }

                onMouseYChanged: {
                    if (lineView.isConstructingLine) {
                        lineView.itemAt(lineView.count - 1).followMouse(mouseX, mouseY);
                    }
                }
            }

            Repeater {
                id: lineView

                property bool isConstructingLine: false

                onItemRemoved: isConstructingLine = false

                model: linesModel
                delegate: Line {
                    property int secondNode: model.secondNode

                    x: dotsView.itemAt(dotsModel.indexByID(model.firstNode)).x
                    y: dotsView.itemAt(dotsModel.indexByID(model.firstNode)).y
                    type: model.type

                    onTypeChanged: model.type = type
                    onSecondNodeChanged: {
                        model.secondNode = secondNode
                        if (model.secondNode !== -1) {
                            followMouse(dotsView.itemAt(dotsModel.indexByID(secondNode)).x,
                                        dotsView.itemAt(dotsModel.indexByID(secondNode)).y);
                            lineView.isConstructingLine = false;
                        } else {
                            lineView.isConstructingLine = true;
                        }
                    }
                    onRemove: linesModel.remove(index);
                }
            }

            Repeater {
                id: dotsView

                model: dotsModel
                delegate: Dot {
                    id: dot

                    x: model.x
                    y: model.y

                    type: model.type
                    nodeID: model.nodeID

                    onTypeChanged: model.type = type

                    onClicked: {
                        if (lineView.isConstructingLine)
                        {
                            lineView.isConstructingLine = false;
                            lineView.itemAt(lineView.count - 1).secondNode = nodeID;
                        }
                        else
                        {
                            switch (button) {
                            case "LeftButton":
                                linesModel.append(nodeID);
                                break;
                            case "MiddleButton":
                                type++;
                                break;
                            case "RightButton":
                                linesModel.nodeRemoved(nodeID);
                                dotsModel.remove(index);
                                break;
                            }
                        }
                    }

                    onMousePositionChanged: {
                        if (lineView.isConstructingLine) {
                            lineView.itemAt(lineView.count - 1).followMouse(mouseX + dot.x, mouseY + dot.y);
                        }
                    }
                }
            }
        }
    }
}
