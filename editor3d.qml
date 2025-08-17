import QtQuick 6.0
import QtQuick3D 6.0
import QtQuick.Controls 6.0
import Example 1.0

Item {
    width: 800
    height: 600

    View3D {
        id: view3d
        anchors.fill: parent

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 20, 30)
            eulerRotation: Qt.vector3d(-10, 0, 0)
        }

        DirectionalLight {
            id: light
            eulerRotation: Qt.vector3d(-45, 45, 0)
            castsShadow: true
            brightness: 2.0
        }

        Model {
            id: customCubeModel
            objectName: "customCubeModel"
            geometry: CustomGeometry {} // Removed the size property
            position: Qt.vector3d(0, 0, 0)
            visible: true // Add this line

            materials: PrincipledMaterial {
                        // Set the base color of the road
                        baseColor: "#4A4A4A" // A dark gray color for asphalt

                        // Control how shiny the surface is (0.0 is shiny, 1.0 is matte)
                        roughness: 0.8
                    }
        }

        NumberAnimation {
            id: spinAnim
            target: customCubeModel
            property: "eulerRotation.y"
            from: 0
            to: 360
            duration: 3000
            loops: Animation.Infinite
            running: true
        }
    }

    Button {
        id: toggleSpinButton
        text: "Toggle Spin"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        onClicked: spinAnim.running = !spinAnim.running
    }
}
