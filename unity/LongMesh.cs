using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
public class LongMesh : MonoBehaviour
{
    public Vector3[] points;
    public float width = 0.1f;

    void Start()
    {
        CreateMesh();
    }

    void CreateMesh()
    {
        if (points == null || points.Length < 2)
        {
            Debug.LogError("You need at least two points to create a mesh.");
            return;
        }

        Mesh mesh = new Mesh();
        Vector3[] vertices = new Vector3[points.Length * 2];
        int[] triangles = new int[(points.Length - 1) * 6];
        Vector2[] uv = new Vector2[vertices.Length];

        for (int i = 0; i < points.Length; i++)
        {
            Vector3 forward = Vector3.forward;
            if (i < points.Length - 1)
            {
                forward = points[i + 1] - points[i];
            }
            else if (i > 0)
            {
                forward = points[i] - points[i - 1];
            }
            forward.Normalize();

            Vector3 left = Vector3.Cross(forward, Vector3.up) * width;
            vertices[i * 2] = points[i] - left;
            vertices[i * 2 + 1] = points[i] + left;

            uv[i * 2] = new Vector2(i / (float)points.Length, 0);
            uv[i * 2 + 1] = new Vector2(i / (float)points.Length, 1);

            if (i < points.Length - 1)
            {
                triangles[i * 6 + 0] = i * 2 + 0;
                triangles[i * 6 + 1] = i * 2 + 1;
                triangles[i * 6 + 2] = i * 2 + 2;

                triangles[i * 6 + 3] = i * 2 + 1;
                triangles[i * 6 + 4] = i * 2 + 3;
                triangles[i * 6 + 5] = i * 2 + 2;
            }
        }

        mesh.vertices = vertices;
        mesh.triangles = triangles;
        mesh.uv = uv;
        mesh.RecalculateNormals();

        GetComponent<MeshFilter>().mesh = mesh;
    }

    void OnDrawGizmos()
    {
        if (points == null || points.Length < 2)
        {
            return;
        }

        Gizmos.color = Color.red;
        for (int i = 0; i < points.Length - 1; i++)
        {
            Gizmos.DrawLine(points[i], points[i + 1]);
        }
    }
}

