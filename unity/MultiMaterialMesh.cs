using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
public class MultiMaterialMesh : MonoBehaviour
{
    public Material[] materials;
    Mesh mesh;
    public int submeshIndex =1;
    public int matIndex = 0;

    void Start()
    {
        CreateMesh();
    }

    void CreateMesh()
    {
        mesh = new Mesh();

        // Define vertices
        Vector3[] vertices = new Vector3[]
        {
            new Vector3(0, 0, 0),
            new Vector3(0, 1, 0),
            new Vector3(1, 1, 0),
            new Vector3(1, 0, 0),
            new Vector3(2, 0, 0),
            new Vector3(2, 1, 0)
        };

        // Define triangles for submesh 1
        int[] triangles1 = new int[]
        {
            0, 1, 2,
            0, 2, 3
        };

        // Define triangles for submesh 2
        int[] triangles2 = new int[]
        {
            3, 2, 5,
            3, 5, 4
        };

        // Assign vertices
        mesh.vertices = vertices;

        // Assign submeshes
        mesh.subMeshCount =2;
        mesh.SetTriangles(triangles1, 0);
        mesh.SetTriangles(triangles2, 1);

        // Assign the mesh to the MeshFilter
        GetComponent<MeshFilter>().mesh = mesh;

        // Assign materials
        MeshRenderer renderer = GetComponent<MeshRenderer>();
        //renderer.materials = materials;
        Graphics.DrawMesh(mesh, transform.localToWorldMatrix, materials[0], gameObject.layer, null, 0);
        Graphics.DrawMesh(mesh, transform.localToWorldMatrix, materials[1], gameObject.layer, null, 1);
    }
    //private void Update()
    //{
    //    Graphics.DrawMesh(mesh, transform.localToWorldMatrix, materials[0], gameObject.layer, null, 0);
    //    Graphics.DrawMesh(mesh, transform.localToWorldMatrix, materials[matIndex], gameObject.layer, null, 1);

    //}

    void OnDrawGizmos()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireCube(transform.position, new Vector3(1, 1, 1));

        if (mesh != null && materials[submeshIndex + 1] != null && submeshIndex >= 0 && submeshIndex < mesh.subMeshCount)
        {
            // Draw the specified submesh with the given material
            if (materials[matIndex].SetPass(0))
            {
                Debug.Log("matIndex : " + matIndex);
                Graphics.DrawMesh(mesh, transform.localToWorldMatrix, materials[0], gameObject.layer, null, 0);
                Graphics.DrawMesh(mesh, transform.localToWorldMatrix, materials[matIndex], gameObject.layer, null, 1);
            }
        }
    }
    void Update/*OnRenderObject*/()
    {
        if (mesh != null && materials[submeshIndex + 1] != null && submeshIndex >= 0 && submeshIndex < mesh.subMeshCount)
        {
            // Draw the specified submesh with the given material
            if (materials[matIndex].SetPass(0))
            {
                Debug.Log("matIndex(Update) : " + matIndex);
                Graphics.DrawMesh(mesh, transform.localToWorldMatrix, materials[0], gameObject.layer, null, 0);
                Graphics.DrawMesh(mesh, transform.localToWorldMatrix, materials[matIndex], gameObject.layer, null, 1);
            }
        }
    }

    void OnRenderObject()
    {
        //if (mesh != null && materials[submeshIndex + 1] != null && submeshIndex >= 0 && submeshIndex < mesh.subMeshCount)
        //{
        //    // Draw the specified submesh with the given material
        //    if (materials[matIndex].SetPass(0))
        //    {
        //        Debug.Log("matIndex(OnRenderObject) : " + matIndex);
        //        Graphics.DrawMesh(mesh, transform.localToWorldMatrix, materials[0], gameObject.layer, null, 0);
        //        Graphics.DrawMesh(mesh, transform.localToWorldMatrix, materials[matIndex], gameObject.layer, null, 1);
        //    }
        //}
    }
}

//using UnityEngine;

//public class DrawSubmesh : MonoBehaviour
//{
//    public Mesh mesh;
//    public Material material;
//    public int submeshIndex = 0;

//    void OnRenderObject()
//    {
//        if (mesh != null && material != null && submeshIndex >= 0 && submeshIndex < mesh.subMeshCount)
//        {
//            // Draw the specified submesh with the given material
//            Graphics.DrawMesh(mesh, transform.localToWorldMatrix, material, 0, null, submeshIndex);
//        }
//    }
//}

