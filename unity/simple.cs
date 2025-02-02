using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
public class simple : MonoBehaviour
{
    private void Start()
    {
        CreateMesh();
    }

    private void CreateMesh()
    {
        Mesh mesh = new Mesh();

        // Assign the mesh to the MeshFilter
        GetComponent<MeshFilter>().mesh = mesh;

        // Define vertices
        Vector3[] vertices = new Vector3[]
        {
            new Vector3(0, 0, 0),
            new Vector3(0, 1, 0),
            new Vector3(1, 1, 0),
            new Vector3(1, 0, 0)
        };

        // Define triangles
        int[] triangles = new int[]
        {
            0, 1, 2,
            0, 2, 3
        };

        // Assign vertices and triangles to the mesh
        mesh.vertices = vertices;
        mesh.triangles = triangles;

        // Recalculate normals for lighting
        mesh.RecalculateNormals();
    }
}
