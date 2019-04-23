import QtQuick 2.0

Item {
    id: root

    signal remove()

    property int type: 3

    height: 1

    transform: Rotation {
        id: rot
    }

    function followMouse(mouseX, mouseY) {
        var lengthX = mouseX - root.x;
        var lengthY = mouseY - root.y;

        width = Math.sqrt(lengthX * lengthX + lengthY * lengthY);

        var angle = Math.atan(lengthY / lengthX) * 180 / Math.PI;

        if (mouseX > root.x) {
            rot.angle = angle;
        } else {
            rot.angle = angle + 180;
        }
    }

    Rectangle {
        id: line

        y: -3
        height: 7
        width: parent.width

        color: "green"
        border.width: 1
        border.color: "transparent"

        Image {
            id: leftArrow

            anchors.left: parent.left
            height: 7
            width: 15
            visible: type == 1 || type == 3

            source: "qrc:/images/left-arrow.png"
        }

        Image {
            id: rightArrow

            anchors.right: parent.right
            height: 7
            width: 15
            visible: type == 2 || type == 3

            source: "qrc:/images/right-arrow.png"
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.RightButton | Qt.MiddleButton

            onEntered: line.border.color = "lightgreen"
            onExited:  line.border.color = "transparent"
            onClicked: {
                switch (mouse.button) {
                case Qt.RightButton:
                    root.remove();
                    break;
                case Qt.MiddleButton:
                    type++;
                    if (type > 3) type = 1;
                    break;
                }
            }
        }
    }
}
