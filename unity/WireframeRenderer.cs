using UnityEngine;

[RequireComponent(typeof(MeshFilter))]
public class WireframeRenderer : MonoBehaviour
{
    private MeshFilter meshFilter;
    private Mesh mesh;

    void Start()
    {
        meshFilter = GetComponent<MeshFilter>();
        mesh = meshFilter.mesh;
    }

    void OnRenderObject()
    {
        // Set the material to the default wireframe material
        Material wireframeMaterial = new Material(Shader.Find("Hidden/Internal-Colored"));
        wireframeMaterial.SetPass(0);

        GL.PushMatrix();
        GL.MultMatrix(transform.localToWorldMatrix);

        GL.Begin(GL.LINES);
        GL.Color(Color.green);

        // Draw lines for each edge in the mesh
        for (int i = 0; i < mesh.subMeshCount; i++)
        {
            int[] indices = mesh.GetIndices(i);
            for (int j = 0; j < indices.Length; j += 3)
            {
                DrawLine(mesh.vertices[indices[j]], mesh.vertices[indices[j + 1]]);
                DrawLine(mesh.vertices[indices[j + 1]], mesh.vertices[indices[j + 2]]);
                DrawLine(mesh.vertices[indices[j + 2]], mesh.vertices[indices[j]]);
            }
        }

        GL.End();
        GL.PopMatrix();
    }

    void DrawLine(Vector3 p1, Vector3 p2)
    {
        GL.Vertex(p1);
        GL.Vertex(p2);
    }
}
