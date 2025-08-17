#include "CustomGeometry.h"
#include <QVector3D>   // <-- add this line


CustomGeometry::CustomGeometry()
{
    updateData();
}

void CustomGeometry::updateData()
{
    // Clear any previous geometry data
    clear();
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);

    QVector<QVector3D> vertices;
    QVector<unsigned int> indices;

    // Road settings
    const int segments = 50;
    const float roadLength = 50.0f;
    const float roadWidth = 2.0f;

    // Generate vertices along a sine curve
    for (int i = 0; i <= segments; ++i) {
        float t = float(i) / segments;
        float x = t * roadLength;
        // The sine function creates a curve in the Z-axis
        float z = qSin(t * 3.14f * 2.0f) * 5.0f;
        float y = 0.0f; // The road is flat on the Y=0 plane

        // Append vertices for both the left and right sides of the road
        vertices.append(QVector3D(x, y, z - roadWidth/2.0f)); // left side
        vertices.append(QVector3D(x, y, z + roadWidth/2.0f)); // right side
    }

    // Generate indices for the triangles with correct winding order
    for (int i = 0; i < segments; ++i) {
        int idx = i * 2;

        // First triangle of the segment (indices must be in a consistent order)
        indices.append(idx);     // left current
        indices.append(idx + 1);   // right current
        indices.append(idx + 3);   // right next

        // Second triangle of the segment
        indices.append(idx);     // left current
        indices.append(idx + 3);   // right next
        indices.append(idx + 2);   // left next
    }

    // Convert vertex data from QVector to QByteArray
    QByteArray vData;
    vData.resize(vertices.size() * 3 * sizeof(float));
    float* pv = reinterpret_cast<float*>(vData.data());
    for (const auto& v : vertices) {
        *pv++ = v.x();
        *pv++ = v.y();
        *pv++ = v.z();
    }
    setVertexData(vData);
    setStride(3 * sizeof(float));
    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);
    addAttribute(QQuick3DGeometry::Attribute::IndexSemantic, 0, QQuick3DGeometry::Attribute::U32Type);



    // Convert index data from QVector to QByteArray
    QByteArray iData;
    iData.resize(indices.size() * sizeof(unsigned int));
    unsigned int* pi = reinterpret_cast<unsigned int*>(iData.data());
    for (auto val : indices) {
        *pi++ = val;
    }
    setIndexData(iData);

    // Set the bounding box (optional but good practice)
    setBounds(QVector3D(0, -roadWidth/2.0f, -roadWidth/2.0f - 5.0f),
              QVector3D(roadLength, roadWidth/2.0f, roadWidth/2.0f + 5.0f));
}


bool CustomGeometry::saveOBJ(const QString& filePath) const
{
    qDebug() << "Starting saveOBJ to" << filePath;

    const QByteArray vData = vertexData();
    const QByteArray iData = indexData();

    if (vData.isEmpty()) {
        qWarning() << "Vertex data is empty!";
        return false;
    }

    if (vData.size() % (3 * sizeof(float)) != 0) {
        qWarning() << "Vertex data size invalid! Size:" << vData.size();
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath;
        return false;
    }

    QTextStream out(&file);
    out.setRealNumberNotation(QTextStream::FixedNotation);
    out.setRealNumberPrecision(6);

    // Write vertices
    const float* p = reinterpret_cast<const float*>(vData.constData());
    int vertexCount = vData.size() / (3 * sizeof(float));
    qDebug() << "Vertex count:" << vertexCount;

    for (int i = 0; i < vertexCount; ++i) {
        if (!p) {
            qWarning() << "Vertex pointer is null at index" << i;
            file.close();
            return false;
        }
        out << "v " << p[0] << " " << p[1] << " " << p[2] << "\n";
        qDebug() << "Vertex" << i << ":" << p[0] << p[1] << p[2];
        p += 3;
    }

    // Write faces
    if (!iData.isEmpty()) {
        if (iData.size() % (3 * sizeof(unsigned int)) != 0) {
            qWarning() << "Index data size invalid! Size:" << iData.size();
        } else {
            const unsigned int* pi = reinterpret_cast<const unsigned int*>(iData.constData());
            int indexCount = iData.size() / sizeof(unsigned int);
            qDebug() << "Index count:" << indexCount;

            for (int i = 0; i < indexCount; i += 3) {
                if (!pi) {
                    qWarning() << "Index pointer is null at face starting index" << i;
                    break;
                }

                // Defensive check
                if (pi[0] >= (unsigned int)vertexCount ||
                    pi[1] >= (unsigned int)vertexCount ||
                    pi[2] >= (unsigned int)vertexCount) {
                    qWarning() << "Index out of bounds:" << pi[0] << pi[1] << pi[2];
                    break;
                }

                out << "f " << pi[0]+1 << " " << pi[1]+1 << " " << pi[2]+1 << "\n";
                qDebug() << "Face" << i/3 << ":" << pi[0]+1 << pi[1]+1 << pi[2]+1;
                pi += 3;
            }
        }
    }

    file.close();
    qDebug() << "OBJ saved successfully to" << filePath;
    return true;
}
bool CustomGeometry::saveCube(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for saving dummy cube:" << filePath;
        return false;
    }

    QTextStream out(&file);

    // Hardcoded vertices for a 2x2x2 cube (centered at origin)
    QVector<QVector3D> dummyVertices = {
        {-1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}, // Back face
        {-1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}  // Front face
    };

    // Hardcoded indices for the 12 triangles (6 faces)
    QVector<unsigned int> dummyIndices = {
        // Back face (vertices 1-based in OBJ)
        1, 2, 3,    1, 3, 4,
        // Front face
        5, 7, 6,    5, 8, 7,
        // Bottom face
        1, 5, 6,    1, 6, 2,
        // Top face
        4, 3, 7,    4, 7, 8,
        // Right face
        2, 6, 7,    2, 7, 3,
        // Left face
        1, 4, 8,    1, 8, 5
    };

    out << "# Dummy Cube Exported from Aves Engine\n";
    out << "o DummyCube\n";

    // Write dummy vertices
    for (const auto &v : dummyVertices) {
        out << "v " << v.x() << " " << v.y() << " " << v.z() << "\n";
    }

    // Write dummy faces (OBJ uses 1-based indexing)
    for (int i = 0; i < dummyIndices.size(); i += 3) {
        out << "f " << dummyIndices[i] << " " << dummyIndices[i+1] << " " << dummyIndices[i+2] << "\n";
    }

    qDebug() << "Dummy cube saved to:" << filePath;
    file.close();
    return true;
}
