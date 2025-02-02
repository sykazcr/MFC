using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class DynamicMeshUsingDrawMesh : MonoBehaviour
{
    [SerializeField]
    Material m_mat;
    Mesh m_mesh;

    void Start()
    {
        // ìÆìIMeshê∂ê¨
        m_mesh = new Mesh();
        m_mesh.vertices = new Vector3[] {
            new Vector3 (-0.5f, -0.5f),
            new Vector3 (0.5f, -0.5f),
            new Vector3 (0.5f, 0.5f),
            new Vector3 (-0.5f, 0.5f),
        };
        m_mesh.uv = new Vector2[]
        {
            new Vector2(0,0),
            new Vector2(1f,0),
            new Vector2(1f,1f),
            new Vector2(0,1f),
        };
        m_mesh.triangles = new int[] {
            0, 1, 2,
            0, 2, 3,
        };
        m_mesh.RecalculateNormals();
        m_mesh.RecalculateBounds();
    }
    private void Update()
    {
        Graphics.DrawMesh(m_mesh, Vector3.zero, Quaternion.identity, m_mat, 0);
    }
}
