import QtQuick 2.12

Item {
    id: root

    property int delIndex
    property int type
    property int nodeID

    signal clicked(string button)
    signal mousePositionChanged(int mouseX, int mouseY)

    onTypeChanged: {
        switch (type % 4) {
        case 0:
            dot.color = "green"
            break;
        case 1:
            dot.color = "red"
            break;
        case 2:
            dot.color = "yellow"
            break;
        case 3:
            dot.color = "black"
            break;
        }
    }

    Rectangle {
        id: dot

        x: -3
        y: -3
        width: 6
        height: 6
        color: "green"

        Rectangle {
            id: highlightCircle

            x: -3
            y: -3
            width: 12
            height: 12
            radius: 6
            border.color: "green"
            border.width: 1
            color: "transparent"
            visible: false
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton

            onEntered: highlightCircle.visible = true
            onExited:  highlightCircle.visible = false
            onClicked: {
                switch (mouse.button) {
                case Qt.RightButton:
                    root.clicked("RightButton");
                    break;
                case Qt.LeftButton:
                    root.clicked("LeftButton");
                    break;
                case Qt.MiddleButton:
                    root.clicked("MiddleButton");
                    break;
                }
            }
            onMouseXChanged: root.mousePositionChanged(mouseX, mouseY)
            onMouseYChanged: root.mousePositionChanged(mouseX, mouseY)
        }
    }
}
